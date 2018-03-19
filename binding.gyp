{
  "targets": [
    {
      "target_name": "ads_com",
      "sources": [ 
        "ads-com.cpp",
        "include/TcAdsAPI.h",
        "include/TcAdsDef.h" 
      ], 
      "libraries": [
        "C:\Users\halmigergo\git\ADSAddon\lib\TcAdsDll.lib"
      ],
      "cflags": ["-Wall", "-std=c++11"],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs" : [
		"<!(node -e \"require('nan')\")", 
		"<!(node -e \"require('streaming-worker-sdk')\")"
		]
    }
  ]
}