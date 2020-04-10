
function(init_usd)
    # Adjust PYTHONPATH, PATH
    append_path_to_env_var("PYTHONPATH" "${PXR_USD_LOCATION}/lib/python")
    if(WIN32)
        append_path_to_env_var("PATH" "${PXR_USD_LOCATION}/bin;${PXR_USD_LOCATION}/lib")
    endif()
endfunction()

init_usd()
