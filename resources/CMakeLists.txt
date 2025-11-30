qt_add_translations(MCAS_TRANSLATIONS
    TS_FILES
        translations/MCAS_en.ts
)

qt_add_resources(MCAS_RESOURCE_COLLECTION resources
    PREFIX "/"
    FILES
        resources.qrc
)

set(MCAS_TRANSLATIONS ${MCAS_TRANSLATIONS} PARENT_SCOPE)
set(MCAS_RESOURCE_COLLECTION ${MCAS_RESOURCE_COLLECTION} PARENT_SCOPE)
