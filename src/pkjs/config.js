module.exports = [
    { 
        "type": "heading", 
        "defaultValue": "Preferences" ,
        "size": 3
    }, 
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Vibration"
            },
            {
                "type": "toggle",
                "label": "Warning (5 seconds until token renew)",
                "messageKey": "vib_warn",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "label": "On new token",
                "messageKey": "vib_renew",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Entries"
            },
            {
                "type": "input",
                "label": "Name 01",
                "description": "Name limited to 16 bytes",
                "messageKey": "S00_NAME",
                "defaultValue": "gtest",
                "attributes": {
                    "limit": 16
                }
            },
            {
                "type": "input",
                "label": "Secret 01",
                "description": "Secret limited to 16 bytes. NOTE do NOT include padding '\"'='",
                "messageKey": "S00_SECRET",
                "defaultValue": "MZXW633PN5XW6",
                "attributes": {
                    "limit": 16
                }
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
];
