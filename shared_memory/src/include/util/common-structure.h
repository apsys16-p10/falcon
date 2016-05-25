#ifndef COMMON_STRUCTURE_H
#define COMMON_STRUCTURE_H
#include <stdint.h>

#define MAX_SERVER_COUNT 10

typedef int64_t node_id_t;
typedef uint32_t req_id_t;
typedef uint32_t view_id_t;

typedef struct view_t{
    view_id_t view_id;
    node_id_t leader_id;
    req_id_t req_id;
}view;

typedef struct view_stamp_t{
    view_id_t view_id;
    req_id_t req_id;
}view_stamp;

uint64_t vstol(view_stamp vs);
view_stamp ltovs(uint64_t);
int view_stamp_comp(view_stamp op1,view_stamp op2);

#endif 