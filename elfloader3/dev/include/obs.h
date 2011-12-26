#ifndef __OBSH__
#define __OBSH__

#define Obs_GetPosition(a,b) GetPlayObjPosition((void*)a,b)
#define Obs_GetDuration(a,b) GetPlayObjDuration((void*)a,b)

typedef struct {
  int type;
  void *func;
}OBSevent;


#ifdef NEWSGOLD
#define OBS_EV_NO_MSG 0x00
#define OBS_EV_GetInfoCon 0x01
#define OBS_EV_ConvDestroyed 0x05
#define OBS_EV_ParamChanged 0x06
#define OBS_EV_StatusChanged 0x08
#define OBS_EV_StatusUpdated 0x09
#define OBS_EV_FrameUpdate 0x0A
#define OBS_EV_LoopUpdate 0x0B
#define OBS_EV_Error 0x0C
#define OBS_EV_StopInd 0x13
#define OBS_EV_PauseInd 0x14
#define OBS_EV_ResumeInd 0x15
#define OBS_EV_StopCon 0x16
#define OBS_EV_PauseCon 0x17
#define OBS_EV_ResumeCon 0x18

//great 0x2c
#ifdef ELKA
#define OBS_EV_PrepareCon 0x35
#define OBS_EV_GetCapabilityCon 0x37
#define OBS_EV_EndList 0x66
#else
#define OBS_EV_PrepareCon 0x34
#define OBS_EV_GetCapabilityCon 0x36
#define OBS_EV_EndList 0x57
#endif


#endif 
/*
//  other events for elka (//NSG)
00 ObsCallback_NoMessage // e_obs_ev_no_message
01 ObsCallback_GetInfoConfirmation
02 ObsCallback_ConversionStarted
03 ObsCallback_ConversionCompleted
04 ObsCallback_ConversionAborted
05 ObsCallback_ConversionDestroyed
06 ObsCallback_ParamChanged
07 //unk //ObsCallback_ParamUpdated //unk
08 ObsCallback_StatusChanged  r0 -obs r1-live_mode = %d, r2 -err-code = %d
09 ObsCallback_StatusUpdated
0A ObsCallback_FrameUpdate
0B ObsCallback_LoopUpdate 
0C ObsCallback_Error
0D //e_obs_ev_emb_found
0E //e_obs_ev_emb_processed
0F ObsCallback_OutputReady  (must call Obs_Output_GetData);
10 ObsCallback_InputConsumed
11 ObsCallback_InputMoreData
12 ObsCallback_InputSeek  //setposition
13 Jnf_PlaybackStoppedIndication //e_obs_ev_stop_ind
14 Jnf_PlaybackPausedIndication
15 Jnf_PlaybackResumeIndication
16 Jnf_PlaybackStopConfirmation
17 Jnf_PlaybackPauseConfirmation
18 Jnf_PlaybackResumeConfirmation
19 //e_obs_ev_init_req
1A //e_obs_ev_prepare_req
1B //e_obs_ev_get_header_size
1C //e_obs_ev_data_correct
1D //e_obs_ev_get_info_req"
1E //e_obs_ev_convert_req
1F //e_obs_ev_pause_object_req
20 //e_obs_ev_resume_object_req
21 //e_obs_ev_abort_conversion_req
22 //e_obs_ev_finalise_object_req
23 //e_obs_ev_destroy_object_req
24 //e_obs_ev_stop_object_req
25 //e_obs_ev_rewind_req
26 //e_obs_ev_start_timer_req
27 //e_obs_ev_capability_req
28 //e_obs_ev_passify_object_req
29 //unk
2A //e_obs_ev_timer_ind
2B //e_obs_live_postponed

2C //-  e_obs_ev_mam_performance_ind
2D //2C e_obs_ev_abort_conversion_con
2E //2D e_obs_ev_pause_object_con
2F //2E e_obs_ev_resume_object_con
30 //2F e_obs_ev_finalise_object_con
31 //30 e_obs_ev_destroy_object_con
32 /31/e_obs_ev_stop_object_con
33 //32e_obs_ev_init_con
34 //33 e_obs_ev_cannot_process
35 //34  Jnf_PlaybackPrepareConfirmation
36 //35 e_obs_ev_convert_con
37 //36Jnf_PlaybackCapabilityConfirmation
38 //e_obs_ev_frame_done_object_con
39 //e_obs_ev_mam_prio_start
3A //e_obs_ev_mam_prio_stop
3B //e_obs_ev_mam_prio_pause
3C //e_obs_ev_mam_prio_resume
3D //e_obs_ev_mam_none_req
3E //e_obs_ev_mam_register_req
3F //e_obs_ev_mam_start_req
40 //e_obs_ev_mam_pause_req
41 //e_obs_ev_mam_stop_req
42 //e_obs_ev_mam_resume_req
43 //e_obs_ev_mam_deregister_req
44 //e_obs_ev_mam_register_grant
45 //e_obs_ev_mam_deregister_grant
46 //e_obs_ev_mam_start_grant
47 //e_obs_ev_mam_start_reject
48 //e_obs_ev_mam_start_postpone
49 //e_obs_ev_mam_pause_grant
4A //e_obs_ev_mam_resume_grant
4B //e_obs_ev_mam_resume_reject
4C //e_obs_ev_mam_resume_postpone
4D //e_obs_ev_mam_stop_grant
4E //e_obs_ev_mam_cannotprocessnow
4F //unk
50 //unk
51 //unk
52 //e_obs_ev_mm_vplayer_stop_con
53 //unk
54 //e_obs_ev_mm_vplayer_eos_reached
55 //unk
56 //e_obs_ev_mm_vplayer_init_error
57 //e_obs_ev_mm_vplayer_play_error
58 //e_obs_ev_mm_vplayer_stop_error
59 //e_obs_ev_mm_vplayer_finalise_error
5A //e_obs_ev_mm_vplayer_dec_error
5B //unk
5C //unk
5D //e_obs_ev_mm_stop_con
5E //e_obs_ev_mm_snapshot_req
5F //e_obs_ev_mm_redraw_req
60 //e_obs_ev_mm_av_input_ready
61 // 51 e_obs_ev_mm_finalise_req
62 //e_obs_ev_mm_poll_pv_scheduler_req"
63 //e_obs_ev_mm_poll_pv_scheduler_con
64 //unk
65 //e_obs_ev_tobj_change_state
66 //57 end
//sgold 2f

*/

#endif

