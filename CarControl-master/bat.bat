set/p keystore_path=input .keystore_path:               
    set/p alias_path=input keystore_alias:                            
    set/p unsign_path=input apk_path:                          
    set/p sign_path=output apk_path:                           

    jarsigner -verbose -keystore %keystore_path% -signedjar %sign_path% %unsign_path% %alias_path%

    pause
