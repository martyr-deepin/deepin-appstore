(function() {
    "use strict";
    angular.module("deepin.store")
        .constant("CONFIG", {
            "self": {
                "endpoints": "/endpoints/v1",
                "data": "/data/v1",
            },
            "login": {
                "host": "https://login.deepin.org",
                "request_path": "/oauth2/authorize",
                "logout_path": "/oauth2/logout",
                "origins": {
                    "http://preview.appstore.deepin.test": {
                        "mainland": {
                            "client_id": "623265ff3cf5788d1b24f675ca6aedf05e70ccd2",
                        },
                    },
                    "http://appstore.deepin.org": {
                        "mainland": {
                            "client_id": "b7444f288bfeddc7dc1e63a6e66b99cc36aa1d5c",
                        },
                        "international": {
                            "client_id": "c1b85424b6721ea9678d9f915846ce0c9c86f791",
                        },
                    },
                    "http://appstore.deepin.com":{
                        "professional": {
                            "client_id": "25369cebd30175caf657dbd6ed1112809ab512d8",
                        },
                        "mainland": {
                            "client_id": "b7444f288bfeddc7dc1e63a6e66b99cc36aa1d5c",
                        },
                        "international": {
                            "client_id": "c1b85424b6721ea9678d9f915846ce0c9c86f791",
                        },
                    },
                    "http://elephant.appstore.deepin.com":{
                        "professional": {
                            "client_id": "1f3bc5bb181500262dc88a5a06720265eeb8f216",
                        },
                    },
                    "http://appstore.deepin.test": {
                        "mainland": {
                            "client_id": "e6bf6f88dab14601e373d7805db086c848af1c74",
                        },
                        "international": {
                            "client_id": "724d30c2e51d4d86db9ef2602cd80da47c7e561d",
                        },
                        "professional": {
                            "client_id": "84de531dc0b6ba12f71061d53a955ecb06493558",
                        },
                    },
                },
            },
            "deepinid": {
                "host": "https://api.deepin.org",
            },
        });
})()
