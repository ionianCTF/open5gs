
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inter_freq_target_info.h"

OpenAPI_inter_freq_target_info_t *OpenAPI_inter_freq_target_info_create(
    int dl_carrier_freq,
    OpenAPI_list_t *cell_id_list
)
{
    OpenAPI_inter_freq_target_info_t *inter_freq_target_info_local_var = ogs_malloc(sizeof(OpenAPI_inter_freq_target_info_t));
    ogs_assert(inter_freq_target_info_local_var);

    inter_freq_target_info_local_var->dl_carrier_freq = dl_carrier_freq;
    inter_freq_target_info_local_var->cell_id_list = cell_id_list;

    return inter_freq_target_info_local_var;
}

void OpenAPI_inter_freq_target_info_free(OpenAPI_inter_freq_target_info_t *inter_freq_target_info)
{
    if (NULL == inter_freq_target_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(inter_freq_target_info->cell_id_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(inter_freq_target_info->cell_id_list);
    ogs_free(inter_freq_target_info);
}

cJSON *OpenAPI_inter_freq_target_info_convertToJSON(OpenAPI_inter_freq_target_info_t *inter_freq_target_info)
{
    cJSON *item = NULL;

    if (inter_freq_target_info == NULL) {
        ogs_error("OpenAPI_inter_freq_target_info_convertToJSON() failed [InterFreqTargetInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "dlCarrierFreq", inter_freq_target_info->dl_carrier_freq) == NULL) {
        ogs_error("OpenAPI_inter_freq_target_info_convertToJSON() failed [dl_carrier_freq]");
        goto end;
    }

    if (inter_freq_target_info->cell_id_list) {
    cJSON *cell_id_list = cJSON_AddArrayToObject(item, "cellIdList");
    if (cell_id_list == NULL) {
        ogs_error("OpenAPI_inter_freq_target_info_convertToJSON() failed [cell_id_list]");
        goto end;
    }

    OpenAPI_lnode_t *cell_id_list_node;
    OpenAPI_list_for_each(inter_freq_target_info->cell_id_list, cell_id_list_node)  {
    if (cJSON_AddNumberToObject(cell_id_list, "", *(double *)cell_id_list_node->data) == NULL) {
        ogs_error("OpenAPI_inter_freq_target_info_convertToJSON() failed [cell_id_list]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_inter_freq_target_info_t *OpenAPI_inter_freq_target_info_parseFromJSON(cJSON *inter_freq_target_infoJSON)
{
    OpenAPI_inter_freq_target_info_t *inter_freq_target_info_local_var = NULL;
    cJSON *dl_carrier_freq = cJSON_GetObjectItemCaseSensitive(inter_freq_target_infoJSON, "dlCarrierFreq");
    if (!dl_carrier_freq) {
        ogs_error("OpenAPI_inter_freq_target_info_parseFromJSON() failed [dl_carrier_freq]");
        goto end;
    }

    if (!cJSON_IsNumber(dl_carrier_freq)) {
        ogs_error("OpenAPI_inter_freq_target_info_parseFromJSON() failed [dl_carrier_freq]");
        goto end;
    }

    cJSON *cell_id_list = cJSON_GetObjectItemCaseSensitive(inter_freq_target_infoJSON, "cellIdList");

    OpenAPI_list_t *cell_id_listList;
    if (cell_id_list) {
    cJSON *cell_id_list_local;
    if (!cJSON_IsArray(cell_id_list)) {
        ogs_error("OpenAPI_inter_freq_target_info_parseFromJSON() failed [cell_id_list]");
        goto end;
    }
    cell_id_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(cell_id_list_local, cell_id_list) {
    if (!cJSON_IsNumber(cell_id_list_local)) {
        ogs_error("OpenAPI_inter_freq_target_info_parseFromJSON() failed [cell_id_list]");
        goto end;
    }
    OpenAPI_list_add(cell_id_listList, &cell_id_list_local->valuedouble);
    }
    }

    inter_freq_target_info_local_var = OpenAPI_inter_freq_target_info_create (
        
        dl_carrier_freq->valuedouble,
        cell_id_list ? cell_id_listList : NULL
    );

    return inter_freq_target_info_local_var;
end:
    return NULL;
}

OpenAPI_inter_freq_target_info_t *OpenAPI_inter_freq_target_info_copy(OpenAPI_inter_freq_target_info_t *dst, OpenAPI_inter_freq_target_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_inter_freq_target_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_inter_freq_target_info_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_inter_freq_target_info_free(dst);
    dst = OpenAPI_inter_freq_target_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

