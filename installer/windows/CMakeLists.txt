install(
    DIRECTORY .
    DESTINATION installer/windows    
    FILES_MATCHING 
    PATTERN "*.iss"
    PATTERN "*.ico"
    )

