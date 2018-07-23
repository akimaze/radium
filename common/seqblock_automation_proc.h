#ifndef RADIUM_COMMON_SEQBLOCK_AUTOMATION_PROC_H
#define RADIUM_COMMON_SEQBLOCK_AUTOMATION_PROC_H



extern LANGSPEC void RT_SEQBLOCK_AUTOMATION_called_before_scheduler(void);
extern LANGSPEC void RT_SEQBLOCK_AUTOMATION_called_before_editor(struct SeqTrack *seqtrack);
extern LANGSPEC void RT_SEQBLOCK_AUTOMATION_called_after_scheduler_and_before_audio(void);
extern LANGSPEC void RT_SEQBLOCK_AUTOMATION_called_when_player_stopped(void);

#ifdef __cplusplus

struct SeqblockAutomation *SEQBLOCK_AUTOMATION_create(struct SeqTrack *seqtrack, struct SeqBlock *seqblock, const dyn_t automation_state, double state_samplerate);
void SEQBLOCK_AUTOMATION_free(struct SeqblockAutomation *seqblockenvelope);
double SEQBLOCK_AUTOMATION_get_db(struct SeqblockAutomation *seqblockenvelope, int nodenum);
double SEQBLOCK_AUTOMATION_get_seqtime(struct SeqblockAutomation *seqblockenvelope, int nodenum);
int SEQBLOCK_AUTOMATION_get_logtype(struct SeqblockAutomation *seqblockenvelope, int nodenum);
int SEQBLOCK_AUTOMATION_get_num_nodes(struct SeqblockAutomation *seqblockenvelope);
int SEQBLOCK_AUTOMATION_add_node(struct SeqblockAutomation *seqblockenvelope, double seqtime, double db, int logtype);
void SEQBLOCK_AUTOMATION_delete_node(struct SeqblockAutomation *seqblockenvelope, int nodenum);
void SEQBLOCK_AUTOMATION_set_curr_node(struct SeqblockAutomation *seqblockenvelope, int nodenum);
void SEQBLOCK_AUTOMATION_cancel_curr_node(struct SeqblockAutomation *seqblockenvelope);
void SEQBLOCK_AUTOMATION_set_curr_automation(struct SeqTrack *seqtrack, struct SeqBlock *seqblock);

// May be called if it there is no current automation.
void SEQBLOCK_AUTOMATION_cancel_curr_automation(void);
void SEQBLOCK_AUTOMATION_set(struct SeqTrack *seqtrack, struct SeqBlock *seqblock, int nodenum, double seqtime, double db, int lotype);
void SEQBLOCK_AUTOMATION_duration_changed(struct SeqBlock *seqblock, int64_t new_duration, radium::PlayerLockOnlyIfNeeded *lock);

////////////////////////

void RT_SEQBLOCK_AUTOMATION_called_after_editor_and_before_audio(const struct SeqTrack *seqtrack, int64_t seqblock_time);
void RT_SEQBLOCK_AUTOMATION_called_when_player_stopped(void);
dyn_t SEQBLOCK_AUTOMATION_get_state(const struct SeqblockAutomation *seqblockenvelope);
void SEQBLOCK_AUTOMATION_apply_state(struct SeqblockAutomation *seqblockenvelope, const dyn_t envelope_state, double state_samplerate);
float SEQBLOCK_AUTOMATION_get_node_x(const struct SeqblockAutomation *seqblockenvelope, int nodenum);
float SEQBLOCK_AUTOMATION_get_node_y(const struct SeqblockAutomation *seqblockenvelope, int seqtracknum, int nodenum);
void SEQBLOCK_AUTOMATION_paint(QPainter *p, const struct SeqBlock *seqblock, float x1, float y1, float x2, float y, bool paint_nodes, float seqblock_x1, float seqblock_x2);

bool RT_seqblock_automation_is_enabled(struct SeqblockAutomation *automation);
bool RT_maybe_get_seqblock_automation_value(struct SeqblockAutomation *automation, double time, double &value);
  
#endif

#endif
