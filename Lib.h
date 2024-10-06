#ifndef _LIB_
#define _LIB_

#include "../inout.c"
#include "../inout.h"
#include "../string.c"
#include "../string.h"
#include "../array/array.h"

int strcmp(const char *s1, const char *s2);


#define type_of(T) _Generic((T), \ 
        char* : "string", \ 
        char : "character", \ 
        int : "int", \ 
        long : "long", \ 
        arr_int*: "array of ints",\
        arr_string*: "array of strings",\
        arr_anything*: "array",\
        arr_double*: "array of doubles",\
        arr_float*: "array of floats",\
        arr_long*: "array of long",\
        arr_unsigned_long*: "array of unsigned longs",\
        arr_unsigned_int*: "array of unsigned ints",\
        File: "a file",\
        Exception: "an exception",\
        default : "void*") 
char *int_help = "An integer";
char *string_help = "A string";
char *long_help = "A long integer";
char *arr_int_help = "An array of ints";
char *arr_string_help = "An array of strings";
char *arr_anything_help = "An array";
char *arr_double_help = "array of doubles";
char *arr_float_help = "array of floats";
char *arr_long_help = "array of long";
char *arr_unsigned_long_help = "array of unsigned longs";
char *arr_unsigned_int_help = "array of unsigned ints";
char *anything_help = "Anything, you should convert it to a type to get more information.";




char *int_functions =  "undefined";
char *string_functions = "undefined";
char *long_functions = "undefined";
char *arr_int_functions = "undefined";
char *arr_string_functions = "undefined";
char *arr_anything_functions = "undefined";
char *arr_double_functions = "undefined";
char *arr_float_functions = "undefined";
char *arr_long_functions =  "undefined";
char *arr_unsigned_long_functions = "undefined";
char *arr_unsigned_int_functions = "undefined";
char *anything_functions = "undefined";

#define help(type) print(type##_help, "\nFunctions: \n", 0); \
    int parens = 0;\
    int count = 0;\
    int i = 0;\
    int previousI = 0;\
    char *funcsList[100];\
    int funcsListCount = 0;\
    while (i<=strlen(type##_functions) - 1){\
        previousI = i;\
        for (; i<=strlen(type##_functions)-1; i++){\
            if (type##_functions[i] == '('){\
                parens++;\
                count++;\
            }else if (type##_functions[i] == ')'){\
                parens--;\
            };\
            if (parens == 0 && count != 0){\
                if(i+1 != strlen(type##_functions)){i++;};break;\
            };\
        };i++;\
        funcsListCount++;\
        count = 0;\
        if (previousI==0){i--;}\
        write(0, type##_functions + previousI, i - previousI);\
        write(0, "\n\n", 2);\
    };
    
#define _expect(T, Ttype, expected_type) if(strcmp(Ttype, expected_type) != 0){write(1, "Expected ", string_len("Expected "));write(1, expected_type, string_len(expected_type));write(1, ", got ", string_len(", got "));write(1, Ttype, string_len(Ttype));write(1, ", on ", 5);write(1, __FILE__, string_len(__FILE__));write(1, ":", 1);write(1, string_int_to_string(__LINE__), string_len(string_int_to_string(__LINE__)));write(1, ".\n", 2);}
#define expect(T, expected_type) _expect(T, type_of(T), expected_type)
#define new(type1, ...) type1##_new(__VA_ARGS__)
#define cast(type1, T) type1##_cast(T, type_of(T))


#define struct(name, help, init, structdefinition, funcdefinition, ...) \
    typedef struct name { \
    structdefinition\
    }name; \
    funcdefinition \
    char *name##_help = help;\
    __VA_ARGS__\
    char *name##_functions = #funcdefinition #__VA_ARGS__;\
    name init
#define Function(ret, name, args, exec) ret _##name(args) {exec}
#define StructFunction(struct, ret, name, args, exec) ret _##struct_##name(struct self, args) {exec}
#define NewFunction(struct, args, exec)  struct##_new(args) {struct self = {};exec;return self;};
#define FunctionPointer(ret, name, args) ret (*name)(args);
#define StructFunctionPointer(struct, ret, name, args) ret (*name)(struct, args);
#define Args(...) __VA_ARGS__
#define Code(...) __VA_ARGS__
#define StructDefinition(...) __VA_ARGS__
#define FunctionDefinition(...) __VA_ARGS__
#define InitFunction(name) self.name = _##name
#define InitStructFunction(struct, name) self.name = _##struct_##name

int int_new(){
    int a;
    return a;
};

string string_new(){
    string a = "";
    return a;
};


struct(File, 
    "A file that can read, write, or append.\n\nReading: \"r\"\nWriting: \"w\"\nAppending: \"a\"\n\n",
    NewFunction(File, Args(char *name, char *type), 
        Code(
            self = (File){};
            self.file = file_open(name, type);
            InitFunction(seek);
            InitStructFunction(File, read);
        )
    ),
    StructDefinition(
        char *name;
        int permissions;
        struct file file;
        FunctionPointer(int, seek, Args(struct File self));
        StructFunctionPointer(struct File, int, read, Args(void *buffer, int size));
    ),
    Function(int, seek, Args(File self), Code(
        return file_get_length(self.file);
    )),
    StructFunction(File, int, read, Args(void *buffer, int size), Code(
        return file_read(self.file, buffer, size);
    ))
)
#endif
#ifndef _STDIO_H_
int sprintf(char * restrict str, const char * restrict format, ...);
#endif

#define Exception(name) char LBC_B[500]; sprintf(LBC_B, "%s:%d: \033[91merror\033[0m: %s\n\nTerminating process with error code -1\n", __FILE__, __LINE__, name);write(1, LBC_B, string_len(LBC_B));exit(-1);
#define UndefinedCastError(type, expected_type) char __string[100];sprintf(__string, "Cast not available for casting object of type %s to a(n) %s", type, expected_type); Exception(__string)
#define File_cast(T, type) strcmp(type, "int") == 0 ? new(File, "", "") : (strcmp(type, "string") == 0 ? new(File, T, "r") : new(File, "", ""))

struct(Exception, // This doesn't have the actual function
"An exception you can raise",
NewFunction(Exception, Args(), Code()),
StructDefinition(),,
)


