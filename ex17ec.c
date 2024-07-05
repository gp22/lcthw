#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_data;
    int max_rows;
    struct Address *rows;
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

        if (conn->db->rows && conn->db->max_rows) {
            for (int i = 0; i < conn->db->max_rows; i++) {
                if (conn->db->rows[i].name) {
                    free(conn->db->rows[i].name);
                    conn->db->rows[i].name = NULL;
                }
                if (conn->db->rows[i].email) {
                    free(conn->db->rows[i].email);
                    conn->db->rows[i].email = NULL;
                }
            }

            free(conn->db->rows);
            conn->db->rows = NULL;
        }

        if (conn->db->rows) {
            free(conn->db->rows);
            conn->db->rows = NULL;
        }

        if (conn->db) {
            free(conn->db);
            conn->db = NULL;
        }

        free(conn);
        conn = NULL;
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

void Address_print(struct Address *addr) {
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

int Calculate_db_size(int max_data, int max_rows) {
    int db_size = sizeof(struct Database)
        + (sizeof(int) * 2 + sizeof(char) * max_data * 2) * max_rows;

    return db_size;
}

void Database_create(struct Connection *conn, const int MAX_DATA, const int MAX_ROWS) {
    conn->db->max_data = MAX_DATA;
    conn->db->max_rows = MAX_ROWS;
    conn->db->rows = malloc(MAX_ROWS * sizeof(struct Address));
    if (!conn->db->rows) {
        die("Memory error", conn);
    }

    int i = 0;
    for (i = 0; i < MAX_ROWS; i++) {
        // Allocate memory for the name field.
        conn->db->rows[i].name = (char *)malloc(MAX_DATA * sizeof(char));
        if (!conn->db->rows[i].name) {
            die("Memory error", conn);
        }

        // Allocate memory for the email field.
        conn->db->rows[i].email = (char *)malloc(MAX_DATA * sizeof(char));
        if (!conn->db->rows[i].email) {
            die("Memory error", conn);
        }

        conn->db->rows[i].id = i;
        conn->db->rows[i].set = 0;
    }
}

void Database_load(struct Connection *conn) {
    rewind(conn->file);

    int rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to load database.", conn);
    }

    rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to load database.", conn);
    }

    Database_create(conn, conn->db->max_data, conn->db->max_rows);

    for (int i = 0; i < conn->db->max_rows; i++) {
        rc = fread(&conn->db->rows[i].id, sizeof(int), 1, conn->file);
        if (rc != 1) {
            die("Failed to load database.", conn);
        }

        rc = fread(&conn->db->rows[i].set, sizeof(int), 1, conn->file);
        if (rc != 1) {
            die("Failed to load database.", conn);
        }

        rc = fread(conn->db->rows[i].name, conn->db->max_data * sizeof(char), 1, conn->file);
        if (rc != 1) {
            die("Failed to load database.", conn);
        }

        rc = fread(conn->db->rows[i].email, conn->db->max_data * sizeof(char), 1, conn->file);
        if (rc != 1) {
            die("Failed to load database.", conn);
        }
    }
}

struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) {
        die("Memory error", NULL);
    }

    conn->db = malloc(sizeof(struct Database));
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

    return conn;
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);

    int rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to write database.", conn);
    }

    rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to write database.", conn);
    }

    for (int i = 0; i < conn->db->max_rows; i++) {
        rc = fwrite(&conn->db->rows[i].id, sizeof(int), 1, conn->file);
        if (rc != 1) {
            die("Failed to write database.", conn);
        }

        rc = fwrite(&conn->db->rows[i].set, sizeof(int), 1, conn->file);
        if (rc != 1) {
            die("Failed to write database.", conn);
        }

        rc = fwrite(conn->db->rows[i].name, conn->db->max_data * sizeof(char), 1, conn->file);
        if (rc != 1) {
            die("Failed to write database.", conn);
        }

        rc = fwrite(conn->db->rows[i].email, conn->db->max_data * sizeof(char), 1, conn->file);
        if (rc != 1) {
            die("Failed to write database.", conn);
        }
    }

    rc = fflush(conn->file);
    if (rc == -1) {
        die("Cannot flush database.", conn);
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];
    if (addr->set) {
        die("Already set. Delete it first.", NULL);
    }

    addr->set = 1;
    int max_data = conn->db->max_data;

    char *res = strncpy(addr->name, name, max_data);

    if (!res) {
        die("Name copy failed.", conn);
    }

    int last_char = strlen(name);
    addr->name[last_char] = '\0';

    res = strncpy(addr->email, email, max_data);

    if (!res) {
        die("Email copy failed.", conn);
    }

    last_char = strlen(email);
    addr->email[last_char] = '\0';

}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set.", conn);
    }
}

void Database_delete(struct Connection *conn, int id) {
    if (conn->db->rows[id].set) {
        conn->db->rows[id].set = 0;

        char *res = strncpy(conn->db->rows[id].name, "\0", 1);
        if (!res) {
            die("Delete failed.", conn);
        }

        res = strncpy(conn->db->rows[id].email, "\0", 1);
        if (!res) {
            die("Delete failed.", conn);
        }
    } else {
        die("ID is not set.", conn);
    }
}

void Database_list(struct Connection *conn) {
    int i = 0;
    struct Database *db = conn->db;
    int max_rows = conn->db->max_rows;

    for (i = 0; i < max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        die("USAGE: ex17ec <dbfile> <action> [action params]", NULL);
    }

    int max_data;
    int max_rows;
    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if (argc > 3 && action != 'c') {
        id = atoi(argv[3]);
    } else if (argc > 4 && action == 'c'){
        max_data = atoi(argv[3]);
        max_rows = atoi(argv[4]);
    }

    if (action != 'c' && id >= conn->db->max_rows) die("There's not that many records.", conn);

    switch (action) {
        case 'c':
            if (argc != 5) die("Need max data and max rows to set", conn);

            Database_create(conn, max_data, max_rows);
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4) die("Need an ID to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6) die("Need ID, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4) die("Need ID to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list", conn);
    }

    if (conn) Database_close(conn);

    return 0;
}
