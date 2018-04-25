#include "cnwn/resource.h"

cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE] = {0};

void cnwn_resource_uninstall_handlers(void)
{
    memset(CNWN_RESOURCE_HANDLERS, 0, sizeof(CNWN_RESOURCE_HANDLERS));
}

void cnwn_resource_install_xml_handlers(void)
{
    
}

int64_t cnwn_resource_extract(cnwn_ResourceType resource_type, cnwn_File * f, int64_t size, cnwn_File * output_f)
{
    if (CNWN_RESOURCE_TYPE_VALID(resource_type) && CNWN_RESOURCE_HANDLERS[resource_type].f_extract != NULL)
        return CNWN_RESOURCE_HANDLERS[resource_type].f_extract(resource_type, f, size, output_f);
    return cnwn_file_copy(f, size, output_f);
}

int64_t cnwn_resource_archive(cnwn_ResourceType resource_type, cnwn_File * f, int64_t size, cnwn_File * input_f)
{
    if (CNWN_RESOURCE_TYPE_VALID(resource_type) && CNWN_RESOURCE_HANDLERS[resource_type].f_archive != NULL)
        return CNWN_RESOURCE_HANDLERS[resource_type].f_archive(resource_type, f, size, input_f);
    return cnwn_file_copy(input_f, size, f);
}


