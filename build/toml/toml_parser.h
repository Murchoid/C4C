// typedef enum
// {
//     TOML_STRING,
//     TOML_INT,
//     TOML_FLOAT,
//     TOML_BOOL,
//     TOML_ARRAY,
//     TOML_TABLE
// } toml_type_t;

// typedef struct toml_value toml_value_t;
// typedef struct toml_array toml_array_t;
// typedef struct toml_table toml_table_t;

// struct toml_value
// {
//     toml_type_t type;

//     union
//     {
//         std::string *string; // "hello"
//         long integer;        // 42
//         double floating;     // 3.14
//         int boolean;         // 0 or 1

//         toml_array_t *array; // [1, 2, 3]
//         toml_table_t *table; // [section]
//     } as;
// };

// struct toml_array
// {
//     toml_value_t **items;
//     size_t count;
//     size_t capacity;
// };

// typedef struct
// {
//     std::string *key;
//     toml_value_t *value;
// } toml_kv_pair_t;

// struct toml_table
// {
//     toml_kv_pair_t *entries;
//     size_t count;
//     size_t capacity;
// };

// typedef struct
// {
//     toml_table_t *root;
// } toml_document_t;