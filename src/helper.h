#ifndef __HIBUS_BUSYBOX_HELPER__
#define __HIBUS_BUSYBOX_HELPER__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Subroutine:  get_usr_id
 * Function:    Get UID according to user name
 * Input:       name[I], user name
 * Output:      UID, or 0xFFFFFFFF for error
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
uid_t get_usr_id(const char *name);


/****************************************************************************
 * Subroutine:  change_euid
 * Function:    Change EUID to invoking endpoint host
 * Input:       from_endpoint[I], invoker endpoint
 *				euid[I], invoker euid
 * Output:      none
 * Description: If invoking endpoint host is localhost, change euid to invoker;
 *				If it is from network, change euid to that of nobody.
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void change_euid(const char *from_endpoint, uid_t euid);


/****************************************************************************
 * Subroutine:  restore_euid
 * Function:    Restore EUID to Hibus owner
 * Input:       none
 * Output:      none
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void restore_euid(void);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_HELPER__
