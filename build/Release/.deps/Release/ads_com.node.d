cmd_Release/ads_com.node := c++ -bundle -undefined dynamic_lookup -Wl,-no_pie -Wl,-search_paths_first -mmacosx-version-min=10.7 -arch x86_64 -L./Release -stdlib=libc++  -o Release/ads_com.node Release/obj.target/ads_com/ads-com.o 