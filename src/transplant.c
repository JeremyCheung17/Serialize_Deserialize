#include "global.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 *
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in global.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/*
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable 
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int path_init(char *name) {
    int length = 1;
    char *x= name;
    char *y = path_buf;
    int i = 0;
    int j = 0;
    if(*x == '/')
    {
        i++;
    }
    while(*(x + i) != '\0')
    {
        if(*(x + i) == '/' && *(x + i - 1) == '/')
        {
            i++;
        }
        length++;
        *(y + j) = *(x + i);
        i++;
        j++;
        if(length > PATH_MAX)
        {
            fputs("Length Exceeds Max Path", stderr);
            return -1;
        }
    }
    if(*(y + j - 1) == '/')
    {
        *(y + j - 1) = '\0';
    }
    else
    {
        *(y + j) = '\0';
    }
    path_length = length - 1;
    return 0;
}

/*
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 *
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */
int path_push(char *name) {
    int length = 1;
    char *x= name;
    char *y = path_buf;
    *(y + path_length) = '/';
    int i = 0;
    while(*(x + i) != '\0')
    {
        length++;
        i++;
        if(length + path_length > PATH_MAX || *(x + i) == '/')
        {
            fputs("Length Exceeds Max Path or name includes /", stderr);
        return -1;
        }
    }
    for(int j = 0; j < length; j++)
    {
        *(y + path_length + j + 1) = *(x + j);
    }
    path_length = path_length + length;
    return 0;
}

/*
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */
int path_pop() {
    char *x= (path_buf + path_length - 1);
    int i = 0;
    int y = path_length;
    if(path_length == 0)
    {
        fputs("Empty Path", stderr);
        return -1;
    }
    while(*(x - i) != '/' && i < y)
    {
        *(x-i) = '\0';
        path_length--;
        i++;
    }
    if(i < y)
    {
        *(x-i) = '\0';
        path_length--;
    }
    return 0;
}
int deserialize_magic()
{
    if(fgetc(stdin) != 0x0c)
    {
        fputs("1st Magic Missing", stderr);
        return -1;
    }
    if(fgetc(stdin) != 0x0d)
    {
        fputs("Second Magic Missing", stderr);
        return -1;
    }
    if(fgetc(stdin) != 0xed)
    {
        fputs("Third Magic Missing", stderr);
        return -1;
    }
    return 0;
}
int deserialize_start_trans()
{
    if(deserialize_magic() == -1)
    {
        fputs("Bad Magic DESER Start Trans", stderr);
        return -1;
    }
    if(fgetc(stdin) != 0x00)
    {
        fputs("Start Trans has incorrect record data", stderr);
        return -1;
    }
    unsigned long long x = 0;
    for(int i = 0; i < 4; i++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            return -1;
        }
        x = (x << 8) | (unsigned char)y;
    }
    if (x != 0)
    {
        return -1;
    }
    unsigned long long z = 0;
    for (int j = 0; j < 8; j++)
    {
        char a = fgetc(stdin);
        z = (z << 8) | (unsigned char)a;
    }
    if(z != 16)
    {
        return -1;
    }
    return 0;
}
int deserialize_end_trans()
{
    if(deserialize_magic() == -1)
    {
        fputs("Bad Magic Deser End trans", stderr);
        return -1;
    }
    if(fgetc(stdin) != 0x01)
    {
        fputs("End trans has incorrect record data", stderr);
        return -1;
    }
    unsigned long long x = 0;
    for(int i = 0; i < 4; i++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            fputs("Missing data", stderr);
            return -1;
        }
        x = (x << 8) | (unsigned char)y;
    }
    if (x != 0)
    {
        fputs("Incorrect Depth", stderr);
        return -1;
    }
    unsigned long long z = 0;
    for (int j = 0; j < 8; j++)
    {
        char a = fgetc(stdin);
        z = (z << 8) | (unsigned char)a;
    }
    if(z != 16)
    {
        fputs("Incorrect Size", stderr);
        return -1;
    }
    return 0;
}
int deserialize_start_dir(unsigned long long depth)
{
    if(deserialize_magic() == -1)
    {
        fputs("Bad Magic Deser Start Dir", stderr);
        return -1;
    }
    if(fgetc(stdin) != 0x02)
    {
        fputs("Start Dir Incorrect Record Entry", stderr);
        return -1;
    }
    unsigned long long x = 0;
    for(int i = 0; i < 4; i++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            fputs("Missing data", stderr);
            return -1;
        }
        x = (x << 8) | (unsigned char)y;
    }
    if (x != depth)
    {
        fputs("Incorrect Depth", stderr);
        return -1;
    }
    unsigned long long z = 0;
    for (int j = 0; j < 8; j++)
    {
        char a = fgetc(stdin);
        z = (z << 8) | a;
    }
    if(z != 16)
    {
        fputs("Incorrect Size", stderr);
        return -1;
    }
    return 0;
}
int deserialize_end_dir(unsigned long long depth)
{
    unsigned long long x = 0;
    for(int i = 0; i < 4; i++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            fputs("Missing Data", stderr);
            return -1;
        }
        x = (x << 8) | (unsigned char)y;
    }
    if (x != depth)
    {
        fputs("Incorrect Depth", stderr);
        return -1;
    }
    unsigned long long z = 0;
    for (int j = 0; j < 8; j++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            fputs("Missing Data", stderr);
            return -1;
        }
        z = (z << 8) | (unsigned char)y;
    }
    if(z != 16)
    {
        fputs("Incorrect Size", stderr);
        return -1;
    }
    return 0;
}
/*
 * @brief Deserialize directory contents into an existing directory.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory.  It reads (from the standard input) a sequence of DIRECTORY_ENTRY
 * records bracketed by a START_OF_DIRECTORY and END_OF_DIRECTORY record at the
 * same depth and it recreates the entries, leaving the deserialized files and
 * directories within the directory named by path_buf.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * each of the records processed.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including depth fields in the records read that do not match the
 * expected value, the records to be processed to not being with START_OF_DIRECTORY
 * or end with END_OF_DIRECTORY, or an I/O error occurs either while reading
 * the records from the standard input or in creating deserialized files and
 * directories.
 */
int deserialize_directory(int depth) {
    if(deserialize_start_dir(depth) == -1)
    {
        fputs("Deser Start Dir Fail", stderr);
        return -1;
    }
    int f = 1;
    while(f == 1)
    {
        if(deserialize_magic() == -1)
        {
            fputs("Bad Magic", stderr);
            return -1;
        }
        int record = fgetc(stdin);
        if(record == 0x03)
        {
            deserialize_end_dir(depth);
            break;
        }
        else
        {
            unsigned long long depth2 = 0;
            for(int i = 0; i < 4; i++)
            {
                char y = fgetc(stdin);
                if(y == EOF)
                {
                    fputs("Missing Data", stderr);
                    return -1;
                }
                depth2 = (depth2 << 8) | (unsigned char)y;
            }
            if(depth2 != depth)
            {
                return -1;
            }
            unsigned long long size = 0;
            for (int j = 0; j < 8; j++)
            {
                char y = fgetc(stdin);
                if(y == EOF)
                {
                    fputs("Missing Data", stderr);
                    return -1;
                }
                size = (size << 8) | (unsigned char)y;
            }
            unsigned long long mode = 0;
            for(int i = 0; i < 4; i++)
            {
                char y = fgetc(stdin);
                if(y == EOF)
                {
                    fputs("Missing Data", stderr);
                    return -1;
                }
                mode = (mode << (8)) | (unsigned char)y;
            }
            unsigned long long entry_size = 0;
            for (int j = 0; j < 8; j++)
            {
                char y = fgetc(stdin);
                if(y == EOF)
                {
                    fputs("Missing Data", stderr);
                    return -1;
                }
                entry_size = (entry_size << 8) | (unsigned char)y;
            }
            int x = size - 16;
            x = x - 12;
            char *z = name_buf;
            int k = 0;
            while(k < x)
            {
                int c = fgetc(stdin);
                *(z + k) = c;
                k++;
            }
            *(z + k) = '\0';
            if(path_push(name_buf) != -1)
            {
                if(global_options != 12)
                {
                    struct stat stat_buf;
                    stat(path_buf, &stat_buf);
                    if(S_ISDIR(mode))
                    {
                        if(S_ISDIR(stat_buf.st_mode))
                        {
                            fputs("No Clob Dupe DIr Name", stderr);
                            return -1;
                        }
                    }
                    else
                    {
                        if(stat(path_buf, &stat_buf) == 0)
                        {
                            fputs("No Clob same File Name", stderr);
                            return -1;
                        }
                    }
                }
                if(S_ISDIR(mode))
                {
                    mkdir(path_buf, 0700);
                    if(deserialize_directory(depth + 1) == -1)
                    {
                        fputs("Deserialize Dir Fail", stderr);
                        return -1;
                    }
                    chmod(path_buf, 0777);
                }
                else
                {
                    if(deserialize_file(depth) == -1)
                    {
                        fputs("Deserialize Dir Fail", stderr);
                        return -1;
                    }
                    chmod(path_buf, 0777);
                }
            }
            else
            {
                fputs("Path Push Fail", stderr);
                return -1;
            }
        }
        path_pop();
    }
    return 0;
}

/*
 * @brief Deserialize the contents of a single file.
 * @details  This function assumes that path_buf contains the name of a file
 * to be deserialized.  The file must not already exist, unless the ``clobber''
 * bit is set in the global_options variable.  It reads (from the standard input)
 * a single FILE_DATA record containing the file content and it recreates the file
 * from the content.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * the FILE_DATA record.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including a depth field in the FILE_DATA record that does not match
 * the expected value, the record read is not a FILE_DATA record, the file to
 * be created already exists, or an I/O error occurs either while reading
 * the FILE_DATA record from the standard input or while re-creating the
 * deserialized file.
 */
int deserialize_file(int depth) {
    if(deserialize_magic() == -1)
    {
        fputs("Bad Magic", stderr);
        return -1;
    }
    if(fgetc(stdin) != 0x05)
    {
        fputs("Incorrect record data for file data", stderr);
        return -1;
    }
    unsigned long long x = 0;
    for(int i = 0; i < 4; i++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            fputs("Missing Data", stderr);
            return -1;
        }
        x = (x << 8) | y;
    }
    if (x != depth)
    {
        fputs("Incorrect Depth", stderr);
        return -1;
    }
    unsigned long long z = 0;
    for (int j = 0; j < 8; j++)
    {
        char y = fgetc(stdin);
        if(y == EOF)
        {
            fputs("Missing Data", stderr);
            return -1;
        }
        z = (z << 8) | y;
    }
    int h = z - 16;
    FILE *f = fopen(path_buf, "w");
    for(int i = 0; i < h; i++)
    {
        int m = fgetc(stdin);
        if(m == EOF)
        {
            fputs("Missing Data", stderr);
            return -1;
        }
        fputc(m,f);
    }
    fclose(f);
    return 0;
}

/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
int serialize_magic()
{
    fputc(0x0C, stdout);
    fputc(0x0D, stdout);
    fputc(0xED, stdout);
    return 0;
}
int serialize_start_trans()
{
    serialize_magic();
    fputc(0x00, stdout);
    fputc(0UL>>24 & 0xff, stdout);
    fputc(0UL>>16 & 0xff, stdout);
    fputc(0UL>>8 & 0xff, stdout);
    fputc(0UL & 0xff, stdout);
    fputc(16UL>>56 & 0xff, stdout);
    fputc(16UL>>48 & 0xff, stdout);
    fputc(16UL>>40 & 0xff, stdout);
    fputc(16UL>>32 & 0xff, stdout);
    fputc(16UL>>24 & 0xff, stdout);
    fputc(16UL>>16 & 0xff, stdout);
    fputc(16UL>>8 & 0xff, stdout);
    fputc(16UL & 0xff, stdout);
    return 0;
}
int serialize_end_trans()
{
    serialize_magic();
    fputc(0x01, stdout);
    fputc(0UL>>24 & 0xff, stdout);
    fputc(0UL>>16 & 0xff, stdout);
    fputc(0UL>>8 & 0xff, stdout);
    fputc(0UL & 0xff, stdout);
    fputc(16UL>>56 & 0xff, stdout);
    fputc(16UL>>48 & 0xff, stdout);
    fputc(16UL>>40 & 0xff, stdout);
    fputc(16UL>>32 & 0xff, stdout);
    fputc(16UL>>24 & 0xff, stdout);
    fputc(16UL>>16 & 0xff, stdout);
    fputc(16UL>>8 & 0xff, stdout);
    fputc(16UL & 0xff, stdout);
    return 0;
}
int serialize_start_dir(unsigned long long depth)
{
    serialize_magic();
    fputc(0x02, stdout);
    fputc(depth>>24 & 0xff, stdout);
    fputc(depth>>16 & 0xff, stdout);
    fputc(depth>>8 & 0xff, stdout);
    fputc(depth & 0xff, stdout);
    fputc(16UL>>56 & 0xff, stdout);
    fputc(16UL>>48 & 0xff, stdout);
    fputc(16UL>>40 & 0xff, stdout);
    fputc(16UL>>32 & 0xff, stdout);
    fputc(16UL>>24 & 0xff, stdout);
    fputc(16UL>>16 & 0xff, stdout);
    fputc(16UL>>8 & 0xff, stdout);
    fputc(16UL & 0xff, stdout);
    return 0;
}
int serialize_end_dir(unsigned long long depth)
{
    serialize_magic();
    fputc(0x03, stdout);
    fputc(depth>>24 & 0xff, stdout);
    fputc(depth>>16 & 0xff, stdout);
    fputc(depth>>8 & 0xff, stdout);
    fputc(depth & 0xff, stdout);
    fputc(16UL>>56 & 0xff, stdout);
    fputc(16UL>>48 & 0xff, stdout);
    fputc(16UL>>40 & 0xff, stdout);
    fputc(16UL>>32 & 0xff, stdout);
    fputc(16UL>>24 & 0xff, stdout);
    fputc(16UL>>16 & 0xff, stdout);
    fputc(16UL>>8 & 0xff, stdout);
    fputc(16UL & 0xff, stdout);
    return 0;
}
int serialize_dir_entry(unsigned long long depth, unsigned long long size, unsigned long long mode, char *name)
{
    char *y = name;
    int length = 0;
    int i = 0;
    while(*(y + i) != '\0')
    {
        length++;
        i++;
    }
    unsigned long long x = 16 + 12 + length;
    serialize_magic();
    fputc(0x04, stdout);
    fputc(depth>>24 & 0xff, stdout);
    fputc(depth>>16 & 0xff, stdout);
    fputc(depth>>8 & 0xff, stdout);
    fputc(depth & 0xff, stdout);
    fputc(x>>56 & 0xff, stdout);
    fputc(x>>48 & 0xff, stdout);
    fputc(x>>40 & 0xff, stdout);
    fputc(x>>32 & 0xff, stdout);
    fputc(x>>24 & 0xff, stdout);
    fputc(x>>16 & 0xff, stdout);
    fputc(x>>8 & 0xff, stdout);
    fputc(x & 0xff, stdout);
    fputc(mode>>24 & 0xff, stdout);
    fputc(mode>>16 & 0xff, stdout);
    fputc(mode>>8 & 0xff, stdout);
    fputc(mode & 0xff, stdout);
    fputc(size>>56 & 0xff, stdout);
    fputc(size>>48 & 0xff, stdout);
    fputc(size>>40 & 0xff, stdout);
    fputc(size>>32 & 0xff, stdout);
    fputc(size>>24 & 0xff, stdout);
    fputc(size>>16 & 0xff, stdout);
    fputc(size>>8 & 0xff, stdout);
    fputc(size & 0xff, stdout);
    char *z = name;
    int j = 0;
    while(*(z + j) != '\0')
    {
        fputc(*(z + j), stdout);
        j++;
    }
    return 0;
}

/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
int serialize_directory(int depth) {
    serialize_start_dir(depth);
    DIR *dir = opendir(path_buf);
    struct dirent *de;
    while((de = readdir(dir)) != NULL)
    {
        if(*(de->d_name) != '.')
        {
            if(path_push(de->d_name)!=-1)
            {
                struct stat stat_buf;
                stat(path_buf, &stat_buf);
                serialize_dir_entry(depth, stat_buf.st_size, stat_buf.st_mode, de->d_name);
                if(S_ISDIR(stat_buf.st_mode))
                {
                    if(serialize_directory(depth + 1) == -1)
                    {
                        fputs("Serialize Dir Failure", stderr);
                        return -1;
                    }
                }
                else if(S_ISREG(stat_buf.st_mode))
                {
                    if(serialize_file(depth, stat_buf.st_size) == -1)
                    {
                        fputs("Serialize File Failure", stderr);
                        return -1;
                    }
                }
                path_pop();
            }
        }
    }
    serialize_end_dir(depth);
    closedir(dir);
    return 0;
}

/*
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
int serialize_file(int depth, off_t size)
{
    unsigned long long  x = size + 16;
    serialize_magic();
    fputc(0x05, stdout);
    fputc(depth>>24 & 0xff, stdout);
    fputc(depth>>16 & 0xff, stdout);
    fputc(depth>>8 & 0xff, stdout);
    fputc(depth & 0xff, stdout);
    fputc(x>>56 & 0xff, stdout);
    fputc(x>>48 & 0xff, stdout);
    fputc(x>>40 & 0xff, stdout);
    fputc(x>>32 & 0xff, stdout);
    fputc(x>>24 & 0xff, stdout);
    fputc(x>>16 & 0xff, stdout);
    fputc(x>>8 & 0xff, stdout);
    fputc(x & 0xff, stdout);
    FILE *f = fopen(path_buf, "r");
    if(f == NULL)
    {
        fputs("Missing File", stderr);
        return -1;
    }
    char y;
    while((y = fgetc(f)) != EOF)
    {
        fputc(y,stdout);
    }
    fclose(f);
    return 0;
}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
int serialize() {
    serialize_start_trans();
    if(serialize_directory(1) == -1)
    {
        fputs("Serialize Directory Fail", stderr);
        return -1;
    }
    serialize_end_trans();
    return 0;
}
/**
 * @brief Reads serialized data from the standard input and reconstructs from it
 * a tree of files and directories.
 * @details  This function assumes path_buf has been initialized with the pathname
 * of a directory into which a tree of files and directories is to be placed.
 * If the directory does not already exist, it is created.  The function then reads
 * from from the standard input a sequence of bytes that represent a serialized tree
 * of files and directories in the format written by serialize() and it reconstructs
 * the tree within the specified directory.  Options that modify the behavior are
 * obtained from the global_options variable.
 *
 * @return 0 if deserialization completes without error, -1 if an error occurs.
 */
int deserialize() {
    mkdir(path_buf, 0700);
    if(deserialize_start_trans() == -1)
    {
        fputs("Deserialize Start Trans Fail", stderr);
        return -1;
    }
    if(deserialize_directory(1) == -1)
    {
        fputs("Deserialize directory Fail", stderr);
        return -1;
    }
    if(deserialize_end_trans() == -1)
    {
        fputs("Deserialize end Trans fail", stderr);
        return -1;
    }
    return 0;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    int moded = 0;
    int d = 0;
    int p = 0;
    int s = 0;
    int i = 1;
    if(argc < 2 || argc > 5)
    {
        fputs("Too few or too many args", stderr);
        return -1;
    }
    while (i < argc)
    {
        char *arg = *(argv + i);
        if(*arg != '-')
        {
            fputs("arg does not begin with -", stderr);
            return -1;
        }
        switch(*(arg + 1))
        {
            case 'h':
                if(moded == 1 || argc > 2)
                {
                    fputs("Mode already selected/more args than h allows", stderr);
                    return -1;
                }
                global_options |= 0x1;
                return 0;
            case 's':
                if(moded == 1)
                {
                    fputs("Mode already selected", stderr);
                    return -1;
                }
                global_options |= 0x2;
                moded = 1;
                s = 1;
                break;
            case 'd':
                if(moded == 1)
                {
                    fputs("Mode Already Selected", stderr);
                    return -1;
                }
                global_options |= 0x4;
                d = 1;
                moded = 1;
                break;
            case 'c':
                if(moded == 0 || d == 0)
                {
                    fputs("No Mode and/or D not selected", stderr);
                    return -1;
                }
                global_options |= 0x8;
                break;
            case 'p':
                if(++i >= argc)
                {
                    fputs("No Path Provided", stderr);
                    return -1;
                }
                p = 1;
                break;
            default:
                fputs("No matching args", stderr);
                return -1;
        }
        i++;
    }

    if(s)
    {
        if(p)
        {
            path_init(*(argv + i - 1));
        }
        else
        {
            path_init(".");
        }
    }
    else if(d)
    {
        if(p)
        {
            path_init(*(argv + i - 1));
        }
        else
        {
            path_init(".");
        }
    }
    return 0;
}
