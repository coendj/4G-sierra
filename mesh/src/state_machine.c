// #include "state_machine.hpp"

// /* State S0 */
// static void s0_entry(void *o)
// {
//         /* Do something */
// }
// static void s0_run(void *o)
// {
//         smf_set_state(SMF_CTX(&s_obj), &demo_states[S1]);
// }
// static void s0_exit(void *o)
// {
//         /* Do something */
// }

// /* State S1 */
// static void s1_run(void *o)
// {
//         smf_set_state(SMF_CTX(&s_obj), &demo_states[S2]);
// }
// static void s1_exit(void *o)
// {
//         /* Do something */
// }

// /* State S2 */
// static void s2_entry(void *o)
// {
//         /* Do something */
// }
// static void s2_run(void *o)
// {
//         smf_set_state(SMF_CTX(&s_obj), &demo_states[S0]);
// }

// /* Populate state table */
// static const struct smf_state demo_states[] = {
//         [S0] = SMF_CREATE_STATE(s0_entry, s0_run, s0_exit),
//         /* State S1 does not have an entry action */
//         [S1] = SMF_CREATE_STATE(NULL, s1_run, s1_exit),
//         /* State S2 does not have an exit action */
//         [S2] = SMF_CREATE_STATE(s2_entry, s2_run, NULL),
// };