#include "../utils.h"

namespace memory {
	headers_t get_file_headers(address_t addr) {
		const auto dos_header = addr.cast<IMAGE_DOS_HEADER*>();
		if (!dos_header
			|| dos_header->e_magic != IMAGE_DOS_SIGNATURE)
			return std::make_pair(nullptr, nullptr);

		const auto nt_headers = addr.self_offset(dos_header->e_lfanew).cast<IMAGE_NT_HEADERS*>();
		if (!nt_headers
			|| nt_headers->Signature != IMAGE_NT_SIGNATURE)
			return std::make_pair(nullptr, nullptr);

		return std::make_pair(dos_header, nt_headers);
	}

	void get_all_modules() {
		const auto peb = reinterpret_cast<_PEB*>(__readfsdword(0x30));
		if (!peb)
			return;

		const auto list = &peb->Ldr->InMemoryOrderModuleList;

		for (auto it = list->Flink; it != list; it = it->Flink) {
			const auto ldr_entry = CONTAINING_RECORD(it, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			if (!ldr_entry)
				continue;

			const auto headers = get_file_headers(ldr_entry->DllBase);
			if (!std::get<IMAGE_DOS_HEADER*>(headers))
				continue;

			const auto module = module_t(ldr_entry, headers);

			m_modules[HASH_RT(module.get_name().c_str())] = module;
		}
	}

	std::vector<int> sig_to_bytes(const char* sig) {
		const auto start = const_cast<char*>(sig);
		const auto end = const_cast<char*>(sig) + strlen(sig);

		auto ret = std::vector<int>();

		for (auto it = start; it < end; it++) {
			if (*it == '?') {
				it++;

				if (*it == '?') {
					it++;
				}

				ret.push_back(-1);
			}
			else {
				ret.push_back(static_cast<int>(strtoul(it, &it, 0x10)));
			}
		}

		return ret;
	}

	address_t find_sig(uint32_t offset, const char* sig, uint32_t range) {
		const auto sig_bytes = sig_to_bytes(sig);
		const auto size = sig_bytes.size();
		const auto data = sig_bytes.data();

		const auto scan_bytes = reinterpret_cast<uint8_t*>(offset);

		for (auto i = 0u; i < range - size; i++) {
			auto found = true;

			for (auto j = 0u; j < size; j++) {
				if (data[j] == -1
					|| scan_bytes[i + j] == data[j])
					continue;

				found = false;
				break;
			}

			if (found)
				return &scan_bytes[i];
		}

		return address_t();
	}

	address_t find_module_sig(uint32_t hash, const char* sig) {
		const auto module = m_modules.at(hash);
		if (!module.m_ldr_entry)
			return address_t();

		return find_sig(module.get_base().cast<uintptr_t>(), sig, module.m_nt_headers->OptionalHeader.SizeOfImage);
	}

	address_t get_export(uint32_t module_hash, uint32_t export_hash) {
		const auto module = m_modules.at(module_hash);
		if (!module.m_ldr_entry)
			return address_t();

		const auto data_directory = &module.m_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		if (!data_directory->VirtualAddress)
			return address_t();

		const auto base = module.get_base();

		const auto export_directory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(base.offset(data_directory->VirtualAddress));
		if (!export_directory)
			return address_t();

		const auto names = reinterpret_cast<uint32_t*>(base.offset(export_directory->AddressOfNames));
		if (!names)
			return address_t();

		const auto funcs = reinterpret_cast<uint32_t*>(base.offset(export_directory->AddressOfFunctions));
		if (!funcs)
			return address_t();

		const auto ords = reinterpret_cast<uint16_t*>(base.offset(export_directory->AddressOfNameOrdinals));
		if (!ords)
			return address_t();

		for (auto i = 0u; i < export_directory->NumberOfNames; i++) {
			if (HASH_RT(reinterpret_cast<const char*>(base.offset(names[i]))) != export_hash)
				continue;

			const auto export_addr = address_t(base.offset(funcs[ords[i]]));

			if (export_addr.cast<uintptr_t>() >= reinterpret_cast<uintptr_t>(export_directory)
				&& export_addr.cast<uintptr_t>() < reinterpret_cast<uintptr_t>(export_directory) + data_directory->Size) {
				const auto forward = std::string(export_addr.cast<const char*>());

				const auto offset = forward.find_last_of('.') + 1u;
				if (offset == std::string::npos)
					return address_t();

				return get_export(HASH_RT(forward.substr(0u, offset).append(_("dll")).c_str()), HASH_RT(forward.substr(offset).c_str()));
			}

			return export_addr;
		}

		return address_t();
	}

	std::unordered_map<uint32_t, module_t> m_modules;
}