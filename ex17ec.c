#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    // char *name;
    // char *email;
    char details[]; // Dynamically allocated in Database_open. 1st max_data bytes for name. 2nd max_data bytes for email.
};

struct Config {
    int max_data;
    int max_rows;
};

struct Database {
    struct Config config;
    struct Address rows[]; // Dynamically allocated in Database_open.
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn) {
    if (conn) {
        if (conn->file) {
            fclose(conn->file);
        }

        if (conn->db) {
            free(conn->db);
        }
    }
}

void die(const char *message, struct Connection *conn) {
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(conn);

    exit(1);
}

void Address_print(struct Address *addr, int max_data) {
    printf("%d %s %s\n", addr->id, addr->details, addr->details + max_data);
}

int Calculate_db_size(int max_data, int max_rows) {
    int db_size = sizeof(struct Database)
        + (sizeof(int) * 2 + sizeof(char) * max_data * 2) * max_rows;

    return db_size;
}

void Database_load(struct Connection *conn) {
    rewind(conn->file);
    size_t db_size;

    if (conn->db->config.max_data && conn->db->config.max_rows) {
        db_size = Calculate_db_size(conn->db->config.max_data, conn->db->config.max_rows);
    } else {
        db_size = sizeof(struct Database);
    }

    int rc = fread(conn->db, db_size, 1, conn->file);

    if (rc != 1) {
        die("Failed to load database.", conn);
    }
}

struct Connection *Database_open(const char *filename, char mode, struct Config *conf) {
    size_t db_size;
    size_t resized_db_size;

    if (mode == 'c') {
        db_size = Calculate_db_size(conf->max_data, conf->max_rows);
    } else {
        db_size = sizeof(struct Database);
    }

    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) {
        die("Memory error", NULL);
    }

    conn->db = malloc(db_size);
    if (!conn->db) {
        die("Memory error", NULL);
    }

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file) {
        die("Failed to open the file", conn);
    }

    if (mode != 'c') {
        // Resize DB after getting max_data and max_rows from file.
        int max_data_db = conn->db->config.max_data;
        int max_rows_db = conn->db->config.max_rows;

        resized_db_size = Calculate_db_size(max_data_db, max_rows_db);
        conn->db = realloc(conn->db, resized_db_size);

        if (!conn->db) {
            die("Memory error", NULL);
        }

        Database_load(conn);
    }

    return conn;
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);
    int max_data = conn->db->config.max_data;
    int max_rows = conn->db->config.max_rows;
    int db_size = Calculate_db_size(max_data, max_rows);
    int rc = fwrite(conn->db, db_size, 1, conn->file);

    if (rc != 1) {
        die("Failed to write database.", conn);

        rc = fflush(conn->file);

        if (rc == -1) {
            die("Cannot flush database.", conn);
        }
    }
}

void Database_create(struct Connection *conn, struct Config *conf) {
    int i = 0;

    for (i = 0; i < conf->max_rows; i++) {
        // Make a prototype to initialize it.
        struct Address addr = {.id = i, .set = 0};
        // Then just assign it.
        conn->db->rows[i] = addr;
    }

    struct Config conf_db = {.max_data = conf->max_data, .max_rows = conf->max_rows};
    conn->db->config = conf_db;
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];
    if (addr->set) {
        die("Already set. Delete it first.", NULL);
    }

    addr->set = 1;
    int max_data = conn->db->config.max_data;

    char *res = strncpy(addr->details, name, max_data);

    if (!res) {
        die("Name copy failed.", conn);
    }

    int last_char = strlen(name);
    addr->details[last_char] = '\0';

    res = strncpy(addr->details + max_data, email, max_data);

    if (!res) {
        die("Email copy failed.", conn);
    }

    last_char = strlen(email);
    addr->details[max_data + last_char] = '\0';

}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        int max_data = conn->db->config.max_data;
        Address_print(addr, max_data);
    } else {
        die("ID is not set.", conn);
    }
}

void Database_delete(struct Connection *conn, int id) {
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
    int i = 0;
    struct Database *db = conn->db;

    for (i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            int max_data = conn->db->config.max_data;
            Address_print(cur, max_data);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        die("USAGE: ex17ec <dbfile> <action> [action params]", NULL);
    }

    struct Connection *conn;
    struct Config conf;
    char *filename = argv[1];
    char action = argv[2][0];
    int id = 0;

    if (argc > 3 && action != 'c') {
        id = atoi(argv[3]);
    } else if (argc > 4 && action == 'c'){
        conf.max_data = atoi(argv[3]);
        conf.max_rows = atoi(argv[4]);
    }
    // if (id >= MAX_ROWS) die("There's not that many records.", conn);

    switch (action) {
        case 'c':
            if (argc != 5) die("Need max data and max rows to set", NULL);

            conn = Database_open(filename, action, &conf);
            Database_create(conn, &conf);
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4) die("Need an ID to get", NULL);

            conn = Database_open(filename, action, NULL);
            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6) die("Need ID, name, email to set", NULL);

            conn = Database_open(filename, action, NULL);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4) die("Need ID to delete", NULL);

            conn = Database_open(filename, action, NULL);
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            conn = Database_open(filename, action, NULL);
            Database_list(conn);
            break;

        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list", NULL);
    }

    if (conn) Database_close(conn);

    return 0;
}
