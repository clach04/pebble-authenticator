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
            },
            {
                "type": "slider",
                "label": "Timeout",
                "description": "Inactivity timeout in secs (zero = no timeout)",
                "messageKey": "TIME_OUT_PERIOD",
                "defaultValue": 120,  // 2 * 60
                "min": 0,
                "max": +720,
                "step": 1
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
