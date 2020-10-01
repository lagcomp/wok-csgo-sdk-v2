#include "../globals.h"

c_key_values::~c_key_values() { interfaces::mem_alloc->free(this); }

i_handle_entity* c_base_handle::get() const { return is_valid() ? interfaces::entity_list->get_client_entity_from_handle(*this) : nullptr; }