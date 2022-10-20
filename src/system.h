#ifndef __HIBUS_BUSYBOX_SYS__
#define __HIBUS_BUSYBOX_SYS__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Subroutine:  sys_register
 * Function:    register system procedure and event for busybox runner
 * Input:       hibus_context[I], Hibus context
 * Output:      None
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void sys_register(hibus_conn *hibus_context);


/****************************************************************************
 * Subroutine:  sys_revoke
 * Function:    revoke system procedure and event for busybox runner
 * Input:       hibus_context[I], Hibus context
 * Output:      None
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void sys_revoke(hibus_conn *hibus_context);


/****************************************************************************
 * Subroutine:  getDiskInfo
 * Function:    get space information of current file system.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * getDiskInfo(PROCEDURE_PARAM);


#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_SYS__
