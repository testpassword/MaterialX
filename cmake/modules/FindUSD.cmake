
if(EXISTS ${PXR_USD_LOCATION}/pxrConfig.cmake)
  include(${PXR_USD_LOCATION}/pxrConfig.cmake)
  set(USD_PLUGINS_DIR ${PXR_USD_LOCATION}/plugin/usd)
  set(USD_PYTHONPATH ${PXR_USD_LOCATION}/lib/python)
  set(USD_GENSCHEMA ${PXR_USD_LOCATION}/bin/usdGenSchema)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(USD
  DEFAULT_MSG
  PXR_INCLUDE_DIRS
  PXR_LIBRARIES
)
