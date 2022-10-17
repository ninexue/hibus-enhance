#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>

#include <hibus.h>
#include <hibox/json.h>

#include "hibus_busybox.h"
#include "interface.h"
#include "helper.h"
#include "filesystem.h"

extern const char *op_errors[];

static struct busybox_procedure fs_procedure[] =
{
	{METHOD_HIBUS_BUSYBOX_LS, 		listDirectory},
	{METHOD_HIBUS_BUSYBOX_RM, 		removeFile},
	{METHOD_HIBUS_BUSYBOX_RMDIR, 	removeDirectory},
	{METHOD_HIBUS_BUSYBOX_MKDIR, 	makeDirectory},
	{METHOD_HIBUS_BUSYBOX_UNLINK, 	unlinkFile},
	{METHOD_HIBUS_BUSYBOX_TOUCH, 	touchFile},
	{METHOD_HIBUS_BUSYBOX_CHDIR, 	changeDirectory}
};

static struct busybox_event fs_event [] = {};

static int get_error_code(void)
{
	int ret = ERROR_BUSYBOX_OK;
	switch(errno)
	{
		case EACCES:
			ret = ERROR_BUSYBOX_EACCES;
			break;
		case EBUSY:
			ret = ERROR_BUSYBOX_EBUSY;
			break;
		case EFAULT:
			ret = ERROR_BUSYBOX_EFAULT;
			break;
		case EIO:
			ret = ERROR_BUSYBOX_EIO;
			break;
		case EISDIR:
			ret = ERROR_BUSYBOX_EISDIR;
			break;
		case ELOOP:
			ret = ERROR_BUSYBOX_ELOOP;
			break;
		case ENAMETOOLONG:
			ret = ERROR_BUSYBOX_ENAMETOOLONG;
			break;
		case ENOENT:
			ret = ERROR_BUSYBOX_ENOENT;
			break;
		case ENOMEM:
			ret = ERROR_BUSYBOX_ENOMEM;
			break;
		case ENOTDIR:
			ret = ERROR_BUSYBOX_ENOTDIR;
			break;
		case EPERM:
			ret = ERROR_BUSYBOX_EPERM;
			break;
		case EROFS:
			ret = ERROR_BUSYBOX_EROFS;
			break;
		case EBADF:
			ret = ERROR_BUSYBOX_EBADF;
			break;
		case EINVAL:
			ret = ERROR_BUSYBOX_EINVAL;
			break;
		case ESRCH:
			ret = ERROR_BUSYBOX_ESRCH;
			break;
		default:
			ret = ERROR_BUSYBOX_UNKONOWN;
			break;
	}

	return ret;
}

char * changeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	const char *path = NULL;
    char dirname[PATH_MAX] = {0, };
	struct stat dict_stat;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_CHDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get working directory
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] == '/')
		{
			strcat(dirname, user->cwd);
			strcat(dirname, "/");
			strcat(dirname, path);
		}
		else
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
	}
	else
		strcpy(dirname, path);

	if(stat(dirname, &dict_stat) < 0)
	{
   		ret_code = ERROR_BUSYBOX_FILE_EXIST;
		goto failed;
	}

	if(!S_ISDIR(dict_stat.st_mode))
	{
   		ret_code = ERROR_BUSYBOX_FILE_EXIST;
		goto failed;
	}

	strcpy(user->cwd, dirname);

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

static bool wildcard_cmp(const char *str, const char *pattern)
{
    int len1 = 0;
    int len2 = 0;
    int mark = 0;
    int p1 = 0;
    int p2 = 0;

    if(str == NULL)
        return false;

    if(pattern == NULL)
        return false;

    len1 = strlen (str);
    len2 = strlen (pattern);

    while((p1 < len1) && (p2 < len2))
	{
        if(pattern[p2] == '?')
		{
            p1++;
            p2++;
            continue;
        }

        if(pattern[p2] == '*')
		{
            p2++;
            mark = p2;
            continue;
        }

        if(str[p1] != pattern[p2])
		{
            if (p1 == 0 && p2 == 0)
                return false;

            p1 -= p2 - mark - 1;
            p2 = mark;
            continue;
        }
        p1++;
        p2++;
    }

    if(p2 == len2)
	{
        if (p1 == len1)
            return true;

        if (pattern[p2 - 1] == '*')
            return true;
    }

    while(p2 < len2)
	{
        if (pattern[p2] != '*')
            return false;

        p2++;
    }

    return true;
}

static void
make_file_list(char *filename, struct stat *file_stat, char *ret_string)
{
	int i = 0;

	sprintf(ret_string + strlen(ret_string),
                "{"
				"\"name\":\"%s\","
				"\"dev\":%ld,"
				"\"node\":%ld,"
				"\"type\":\"",
				filename, file_stat->st_dev, file_stat->st_ino);

	switch(file_stat->st_mode & S_IFMT)
	{
		case S_IFREG:
			strcat(ret_string, "-\",");
			break;
		case S_IFCHR:
			strcat(ret_string, "c\",");
			break;
		case S_IFBLK:
			strcat(ret_string, "b\",");
			break;
		case S_IFDIR:
			strcat(ret_string, "d\",");
			break;
		case S_IFIFO:
			strcat(ret_string, "p\",");
			break;
		case S_IFLNK:
			strcat(ret_string, "l\",");
			break;
		case S_IFSOCK:
			strcat(ret_string, "s\",");
			break;
	}

	strcat(ret_string, "\"mode\":\"");
	for(i = 8; i >= 0; i--)
	{
		if((file_stat->st_mode >> i) & 1)
		{
			switch(i % 3)
			{
				case 2:
					strcat(ret_string, "r");
					break;
				case 1:
					strcat(ret_string, "w");
					break;
				case 0:
					strcat(ret_string, "x");
					break;
			}
		}
		else
			strcat(ret_string, "-");
	}
	strcat(ret_string, "\",");

	sprintf(ret_string + strlen(ret_string),
				"\"link\":%lu,"
				"\"uid\":%u,"
				"\"gid\":%u,"
				"\"rdev\":%lu,"
				"\"size\":%ld,"
				"\"blksize\":%ld,"
				"\"blocks\":%ld,"
				"\"atime\":\"%s\","
				"\"mtime\":\"%s\","
				"\"ctime\":\"%s\""
				"}",
				file_stat->st_nlink, file_stat->st_uid, file_stat->st_gid,
				file_stat->st_rdev, file_stat->st_size, file_stat->st_blksize,
				file_stat->st_blocks, ctime(&(file_stat->st_atime)),
				ctime(&(file_stat->st_mtime)), ctime(&(file_stat->st_ctime)));
}

char * listDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(8192);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
    char dirname[PATH_MAX] = {0, };
    char filename[PATH_MAX] = {0, };
    char wildcard[PATH_MAX] = {0, };
	struct stat file_stat;
	char *tempchr = NULL;
	size_t length = 0;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    sprintf(ret_string, "{\"list\":[");

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_LS,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file name and wildcard
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] != '/')
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
		else
		{
			strcpy(full_path, user->cwd);
			strcat(full_path, "/");
			strcat(full_path, path);
		}
	}
	else
		strcpy(full_path, path);

	tempchr = strrchr(full_path, '/');
	if(tempchr == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
	}
	else
	{
		length = strlen(full_path);
		if(length == (size_t)(tempchr - full_path + 1))
		{
			strcpy(dirname, full_path);
			if(full_path == tempchr)
				dirname[length] = 0;
			else
				dirname[length - 1] = 0;

			if(stat(dirname, &file_stat) < 0)
			{
   				ret_code = ERROR_BUSYBOX_FILE_EXIST;
		        goto failed;
			}

			if(!S_ISDIR(file_stat.st_mode))
			{
   				ret_code = ERROR_BUSYBOX_FILE_EXIST;
		        goto failed;
			}
		}
		else
		{
			if(full_path == tempchr)
			{
				dirname[0] = '/';
				dirname[1] = 0;
			}
			else
			{
				strncpy(dirname, full_path, tempchr - full_path);
				dirname[tempchr - full_path] = 0;
			}
			strcpy(filename, tempchr + 1);

			if(strchr(filename, '*') || strchr(filename, '?'))
			{
				strcpy(wildcard, filename);
				filename[0] = 0;
			}
			else
			{
				if(filename[0])
				{
					if(stat(full_path, &file_stat) < 0)
					{
        				ret_code = ERROR_BUSYBOX_FILE_EXIST;
				        goto failed;
					}

					if(S_ISDIR(file_stat.st_mode))
					{
						if(full_path != tempchr)
							strcat(dirname, "/");
						strcat(dirname, filename);
						filename[0] = 0;
					}
				}
				else
				{
					if(stat(dirname, &file_stat) < 0)
					{
        				ret_code = ERROR_BUSYBOX_FILE_EXIST;
				        goto failed;
					}

					if(!S_ISDIR(file_stat.st_mode))
					{
        				ret_code = ERROR_BUSYBOX_FILE_EXIST;
				        goto failed;
					}
				}
			}
		}
	}


	if(filename[0] == 0)
	{
		DIR *dir = NULL;
		struct dirent *ptr = NULL;
		bool first = true;

		if((dir = opendir(dirname)) == NULL)
		{
        	ret_code = ERROR_BUSYBOX_DIR_OPEN;
        	goto failed;
    	}

    	while((ptr = readdir(dir)) != NULL)
		{
        	if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name, "..") == 0)
            	continue;

			// use wildcard
			if(wildcard[0])
			{
            	if(!wildcard_cmp(ptr->d_name, wildcard))
					continue;
			}

        	strcpy (filename, dirname);
	 	    strcat (filename, "/");
        	strcat (filename, ptr->d_name);

	        if(stat(filename, &file_stat) < 0)
	            continue;

			if(first)
				first = false;
			else
				sprintf(ret_string + strlen(ret_string), ",");

			make_file_list(ptr->d_name, &file_stat, ret_string);
		}

		closedir(dir);
	}
	else
		make_file_list(filename, &file_stat, ret_string);

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string + strlen(ret_string),
			"],\"errCode\":%d, \"errMsg\":\"%s\"}",
			ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;

}

char * removeFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
	const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
	struct stat file_stat;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_RM,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file path
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] != '/')
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
		else
		{
			strcpy(full_path, user->cwd);
			strcat(full_path, "/");
			strcat(full_path, path);
		}
	}
	else
		strcpy(full_path, path);

	if(stat(full_path, &file_stat) < 0)
	{
   		ret_code = ERROR_BUSYBOX_FILE_EXIST;
		goto failed;
	}

	if(S_ISDIR(file_stat.st_mode))
	{
   		ret_code = ERROR_BUSYBOX_NOT_FILE;
		goto failed;
	}

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string,
			"{\"errCode\":%d, \"errMsg\":\"%s\"}",
			ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * removeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
	struct stat file_stat;
	const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_RMDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file path
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] != '/')
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
		else
		{
			strcpy(full_path, user->cwd);
			strcat(full_path, "/");
			strcat(full_path, path);
		}
	}
	else
		strcpy(full_path, path);

	if(stat(full_path, &file_stat) < 0)
	{
   		ret_code = ERROR_BUSYBOX_FILE_EXIST;
		goto failed;
	}

	if(!S_ISDIR(file_stat.st_mode))
	{
   		ret_code = ERROR_BUSYBOX_NOT_DIRECTORY;
		goto failed;
	}

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string,
			"{\"errCode\":%d, \"errMsg\":\"%s\"}",
			ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * makeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
	const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
	struct stat file_stat;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_MKDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file path
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] != '/')
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
		else
		{
			strcpy(full_path, user->cwd);
			strcat(full_path, "/");
			strcat(full_path, path);
		}
	}
	else
		strcpy(full_path, path);

	if(stat(full_path, &file_stat) == 0)
	{
   		ret_code = ERROR_BUSYBOX_FILE_EXIST;
		goto failed;
	}


failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string,
			"{\"errCode\":%d, \"errMsg\":\"%s\"}",
			ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * unlinkFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
	const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
	struct stat file_stat;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_UNLINK,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file path
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] != '/')
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
		else
		{
			strcpy(full_path, user->cwd);
			strcat(full_path, "/");
			strcat(full_path, path);
		}
	}
	else
		strcpy(full_path, path);

	if(stat(full_path, &file_stat) < 0)
	{
   		ret_code = ERROR_BUSYBOX_FILE_EXIST;
		goto failed;
	}

	if(S_ISREG(file_stat.st_mode))
	{
		if(unlink(full_path) != 0)
			ret_code = get_error_code();
	}
	else
	{
   		ret_code = ERROR_BUSYBOX_NOT_FILE;
		goto failed;
	}

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string,
			"{\"errCode\":%d, \"errMsg\":\"%s\"}",
			ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}


char * touchFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
	const char *path = NULL;
    char full_path[PATH_MAX] = {0, };
	struct stat file_stat;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;
	hibus_user *user = (hibus_user *)hibus_conn_get_user_data(conn);

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_TOUCH,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file path
    if(json_object_object_get_ex(jo, "path", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	path = json_object_get_string(jo_tmp);
    if(path == NULL)
	{
        ret_code = ERROR_BUSYBOX_INVALID_PARAM;
        goto failed;
    }

	if(path[0] != '/')
	{
		if(user->cwd[0] != '/')
		{
        	ret_code = ERROR_BUSYBOX_WORKING_DICT;
        	goto failed;
		}
		else
		{
			strcpy(full_path, user->cwd);
			strcat(full_path, "/");
			strcat(full_path, path);
		}
	}
	else
		strcpy(full_path, path);

	if(stat(full_path, &file_stat) == 0)
	{
		if(S_ISDIR(file_stat.st_mode))
		{
   			ret_code = ERROR_BUSYBOX_NOT_FILE;
			goto failed;
		}
	}

	// file not exist, create it
    if(access(full_path, F_OK | R_OK) != 0)
	{
        int fd = -1;
        fd = open(full_path, O_CREAT | O_WRONLY,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |S_IWOTH);

        if(fd != -1)
            close (fd);
        else
            ret_code = ERROR_BUSYBOX_CREATE_FILE;
    }
    else
	{
		// change time
        struct timespec newtime[2];
        newtime[0].tv_nsec = UTIME_NOW;
        newtime[1].tv_nsec = UTIME_NOW;
        if(utimensat(AT_FDCWD, full_path, newtime, 0))
            ret_code = get_error_code();
    }

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string,
			"{\"errCode\":%d, \"errMsg\":\"%s\"}",
			ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}


void fs_register(hibus_conn *context)
{
	size_t i = 0;
	size_t size = sizeof(fs_procedure) / sizeof(struct busybox_procedure);
    int ret_code = 0;

	for(i = 0; i < size; i++)
	{
    	ret_code = hibus_register_procedure(context, fs_procedure[i].name,
										NULL, NULL, fs_procedure[i].handler);
	    if(ret_code)
    	{
        	fprintf(stderr, "Busybox--filesystem: Error for register procedure"
							"%s, %s.\n", fs_procedure[i].name,
							hibus_get_err_message(ret_code));
	        return;
    	}
	}

	size = sizeof(fs_event) / sizeof(struct busybox_event);
	for(i = 0; i < size; i++)
	{
    	ret_code = hibus_register_event(context, fs_event[i].name, NULL, NULL);
	    if(ret_code)
    	{
        	fprintf(stderr, "Busybox--filesystem: Error for register event"
							"%s, %s.\n",
							fs_event[i].name, hibus_get_err_message(ret_code));
	        return;
    	}
	}
}

void fs_revoke(hibus_conn *context)
{
	size_t i = 0;
	size_t size = sizeof(fs_procedure) / sizeof(struct busybox_procedure);

	for(i = 0; i < size; i++)
    	hibus_revoke_procedure(context, fs_procedure[i].name);

	size = sizeof(fs_event) / sizeof(struct busybox_event);
	for(i = 0; i < size; i++)
		hibus_revoke_event(context, fs_event[i].name);

}
