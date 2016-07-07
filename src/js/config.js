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
        "type": "submit",
        "defaultValue": "Save"
    }
];
