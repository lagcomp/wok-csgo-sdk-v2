#pragma once
#include "../../globals.h"

class c_prediction : public c_singleton<c_prediction> {
private:
	struct {
		__forceinline void store() {
			m_cur_time = interfaces::global_vars->m_cur_time;
			m_frame_time = interfaces::global_vars->m_frame_time;

			m_in_prediction = interfaces::prediction->m_in_prediction;
			m_first_time_predicted = interfaces::prediction->m_first_time_predicted;
		}

		__forceinline void restore() {
			interfaces::global_vars->m_cur_time = m_cur_time;
			interfaces::global_vars->m_frame_time = m_frame_time;

			interfaces::prediction->m_in_prediction = m_in_prediction;
			interfaces::prediction->m_first_time_predicted = m_first_time_predicted;
		}

		float m_frame_time, m_cur_time;
		bool m_first_time_predicted, m_in_prediction;
	} m_backup;

	void predict(c_cs_player* player, c_user_cmd* cmd);

	int* m_random_seed;
	c_cs_player* m_player;

	move_data_t* m_move_data;
	float m_spread, m_inaccuracy;
public:
	c_prediction() {
		m_random_seed = *SIG("client.dll", "A3 ? ? ? ? 66 0F 6E 86").self_offset(0x1).cast<int**>();
		m_player = *SIG("client.dll", "89 35 ? ? ? ? F3 0F 10 48").self_offset(0x2).cast<c_cs_player**>();

		m_move_data = reinterpret_cast<move_data_t*>(interfaces::mem_alloc->alloc(sizeof(move_data_t)));
	}

	void update();
	void process(c_cs_player* player, c_user_cmd* cmd);
	void restore();

	float get_spread() const { return m_spread; }
	float get_inaccuracy() const { return m_inaccuracy; }
};
#define engine_prediction c_prediction::instance()
