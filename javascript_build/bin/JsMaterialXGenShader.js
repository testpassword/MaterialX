var MaterialX = (function () {
    var _scriptDir = typeof document !== 'undefined' && document.currentScript ? document.currentScript.src : undefined;
    if (typeof __filename !== 'undefined') _scriptDir = _scriptDir || __filename;
    return (
        function (MaterialX) {
            MaterialX = MaterialX || {};

            var Module = typeof MaterialX !== "undefined" ? MaterialX : {};
            var readyPromiseResolve, readyPromiseReject;
            Module["ready"] = new Promise(function (resolve, reject) {
                readyPromiseResolve = resolve;
                readyPromiseReject = reject
            });
            if (!Module.expectedDataFileDownloads) {
                Module.expectedDataFileDownloads = 0
            }
            Module.expectedDataFileDownloads++;
            (function () {
                var loadPackage = function (metadata) {
                    var PACKAGE_PATH = "";
                    if (typeof window === "object") {
                        PACKAGE_PATH = window["encodeURIComponent"](window.location.pathname.toString().substring(0, window.location.pathname.toString().lastIndexOf("/")) + "/")
                    } else if (typeof process === "undefined" && typeof location !== "undefined") {
                        PACKAGE_PATH = encodeURIComponent(location.pathname.toString().substring(0, location.pathname.toString().lastIndexOf("/")) + "/")
                    }
                    var PACKAGE_NAME = "../../bin/JsMaterialXGenShader.data";
                    var REMOTE_PACKAGE_BASE = "JsMaterialXGenShader.data";
                    if (typeof Module["locateFilePackage"] === "function" && !Module["locateFile"]) {
                        Module["locateFile"] = Module["locateFilePackage"];
                        err("warning: you defined Module.locateFilePackage, that has been renamed to Module.locateFile (using your locateFilePackage for now)")
                    }
                    var REMOTE_PACKAGE_NAME = Module["locateFile"] ? Module["locateFile"](REMOTE_PACKAGE_BASE, "") : REMOTE_PACKAGE_BASE;
                    var REMOTE_PACKAGE_SIZE = metadata["remote_package_size"];
                    var PACKAGE_UUID = metadata["package_uuid"];

                    function fetchRemotePackage(packageName, packageSize, callback, errback) {
                        if (typeof process === "object") {
                            require("fs").readFile(packageName, function (err, contents) {
                                if (err) {
                                    errback(err)
                                } else {
                                    callback(contents.buffer)
                                }
                            });
                            return
                        }
                        var xhr = new XMLHttpRequest;
                        xhr.open("GET", packageName, true);
                        xhr.responseType = "arraybuffer";
                        xhr.onprogress = function (event) {
                            var url = packageName;
                            var size = packageSize;
                            if (event.total) size = event.total;
                            if (event.loaded) {
                                if (!xhr.addedTotal) {
                                    xhr.addedTotal = true;
                                    if (!Module.dataFileDownloads) Module.dataFileDownloads = {};
                                    Module.dataFileDownloads[url] = {loaded: event.loaded, total: size}
                                } else {
                                    Module.dataFileDownloads[url].loaded = event.loaded
                                }
                                var total = 0;
                                var loaded = 0;
                                var num = 0;
                                for (var download in Module.dataFileDownloads) {
                                    var data = Module.dataFileDownloads[download];
                                    total += data.total;
                                    loaded += data.loaded;
                                    num++
                                }
                                total = Math.ceil(total * Module.expectedDataFileDownloads / num);
                                if (Module["setStatus"]) Module["setStatus"]("Downloading data... (" + loaded + "/" + total + ")")
                            } else if (!Module.dataFileDownloads) {
                                if (Module["setStatus"]) Module["setStatus"]("Downloading data...")
                            }
                        };
                        xhr.onerror = function (event) {
                            throw new Error("NetworkError for: " + packageName)
                        };
                        xhr.onload = function (event) {
                            if (xhr.status == 200 || xhr.status == 304 || xhr.status == 206 || xhr.status == 0 && xhr.response) {
                                var packageData = xhr.response;
                                callback(packageData)
                            } else {
                                throw new Error(xhr.statusText + " : " + xhr.responseURL)
                            }
                        };
                        xhr.send(null)
                    }

                    function handleError(error) {
                        console.error("package error:", error)
                    }

                    var fetchedCallback = null;
                    var fetched = Module["getPreloadedPackage"] ? Module["getPreloadedPackage"](REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE) : null;
                    if (!fetched) fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE, function (data) {
                        if (fetchedCallback) {
                            fetchedCallback(data);
                            fetchedCallback = null
                        } else {
                            fetched = data
                        }
                    }, handleError);

                    function runWithFS() {
                        function assert(check, msg) {
                            if (!check) throw msg + (new Error).stack
                        }

                        Module["FS_createPath"]("/", "libraries", true, true);
                        Module["FS_createPath"]("/libraries", "adsk", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "adsklib", true, true);
                        Module["FS_createPath"]("/libraries/adsk/adsklib", "genglsl", true, true);
                        Module["FS_createPath"]("/libraries/adsk/adsklib", "genmdl", true, true);
                        Module["FS_createPath"]("/libraries/adsk/adsklib", "genosl", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "lights", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "materials", true, true);
                        Module["FS_createPath"]("/libraries/adsk/materials", "wood", true, true);
                        Module["FS_createPath"]("/libraries/adsk/materials/wood", "parquet_3", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "procedurals", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "shaders", true, true);
                        Module["FS_createPath"]("/libraries/adsk/shaders", "brick", true, true);
                        Module["FS_createPath"]("/libraries/adsk/shaders", "plastic", true, true);
                        Module["FS_createPath"]("/libraries/adsk/shaders", "wood", true, true);
                        Module["FS_createPath"]("/libraries/adsk/shaders/wood", "parquet_3", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "textures", true, true);
                        Module["FS_createPath"]("/libraries/adsk/textures", "bricks_02", true, true);
                        Module["FS_createPath"]("/libraries/adsk/textures/bricks_02", "2k", true, true);
                        Module["FS_createPath"]("/libraries/adsk/textures", "bricks_10", true, true);
                        Module["FS_createPath"]("/libraries/adsk/textures/bricks_10", "2k", true, true);
                        Module["FS_createPath"]("/libraries/adsk/textures", "parquet_03", true, true);
                        Module["FS_createPath"]("/libraries/adsk/textures/parquet_03", "2k", true, true);
                        Module["FS_createPath"]("/libraries/adsk", "user", true, true);
                        Module["FS_createPath"]("/libraries", "bxdf", true, true);
                        Module["FS_createPath"]("/libraries/bxdf", "lama", true, true);
                        Module["FS_createPath"]("/libraries/bxdf", "translation", true, true);
                        Module["FS_createPath"]("/libraries", "lights", true, true);
                        Module["FS_createPath"]("/libraries/lights", "genglsl", true, true);
                        Module["FS_createPath"]("/libraries", "pbrlib", true, true);
                        Module["FS_createPath"]("/libraries/pbrlib", "genglsl", true, true);
                        Module["FS_createPath"]("/libraries/pbrlib/genglsl", "lib", true, true);
                        Module["FS_createPath"]("/libraries/pbrlib/genglsl", "ogsfx", true, true);
                        Module["FS_createPath"]("/libraries/pbrlib", "genmdl", true, true);
                        Module["FS_createPath"]("/libraries/pbrlib", "genosl", true, true);
                        Module["FS_createPath"]("/libraries/pbrlib/genosl", "lib", true, true);
                        Module["FS_createPath"]("/libraries", "stdlib", true, true);
                        Module["FS_createPath"]("/libraries/stdlib", "genglsl", true, true);
                        Module["FS_createPath"]("/libraries/stdlib/genglsl", "lib", true, true);
                        Module["FS_createPath"]("/libraries/stdlib", "genmdl", true, true);
                        Module["FS_createPath"]("/libraries/stdlib", "genosl", true, true);
                        Module["FS_createPath"]("/libraries/stdlib/genosl", "lib", true, true);
                        Module["FS_createPath"]("/libraries/stdlib", "osl", true, true);
                        Module["FS_createPath"]("/libraries", "targets", true, true);

                        function DataRequest(start, end, audio) {
                            this.start = start;
                            this.end = end;
                            this.audio = audio
                        }

                        DataRequest.prototype = {
                            requests: {}, open: function (mode, name) {
                                this.name = name;
                                this.requests[name] = this;
                                Module["addRunDependency"]("fp " + this.name)
                            }, send: function () {
                            }, onload: function () {
                                var byteArray = this.byteArray.subarray(this.start, this.end);
                                this.finish(byteArray)
                            }, finish: function (byteArray) {
                                var that = this;
                                Module["FS_createDataFile"](this.name, null, byteArray, true, true, true);
                                Module["removeRunDependency"]("fp " + that.name);
                                this.requests[this.name] = null
                            }
                        };
                        var files = metadata["files"];
                        for (var i = 0; i < files.length; ++i) {
                            new DataRequest(files[i]["start"], files[i]["end"], files[i]["audio"]).open("GET", files[i]["filename"])
                        }

                        function processPackageData(arrayBuffer) {
                            assert(arrayBuffer, "Loading data file failed.");
                            assert(arrayBuffer instanceof ArrayBuffer, "bad input to processPackageData");
                            var byteArray = new Uint8Array(arrayBuffer);
                            DataRequest.prototype.byteArray = byteArray;
                            var files = metadata["files"];
                            for (var i = 0; i < files.length; ++i) {
                                DataRequest.prototype.requests[files[i].filename].onload()
                            }
                            Module["removeRunDependency"]("datafile_../../bin/JsMaterialXGenShader.data")
                        }

                        Module["addRunDependency"]("datafile_../../bin/JsMaterialXGenShader.data");
                        if (!Module.preloadResults) Module.preloadResults = {};
                        Module.preloadResults[PACKAGE_NAME] = {fromCache: false};
                        if (fetched) {
                            processPackageData(fetched);
                            fetched = null
                        } else {
                            fetchedCallback = processPackageData
                        }
                    }

                    if (Module["calledRun"]) {
                        runWithFS()
                    } else {
                        if (!Module["preRun"]) Module["preRun"] = [];
                        Module["preRun"].push(runWithFS)
                    }
                };
                loadPackage({
                    "files": [{
                        "filename": "/libraries/CMakeLists.txt",
                        "start": 0,
                        "end": 185,
                        "audio": 0
                    }, {
                        "filename": "/libraries/README.md",
                        "start": 185,
                        "end": 5289,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/adsklib_defs.mtlx",
                        "start": 5289,
                        "end": 16021,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/adsklib_ng.mtlx",
                        "start": 16021,
                        "end": 59243,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/adsk_colorcorrect.mtlx",
                        "start": 59243,
                        "end": 65650,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/genglsl/adsklib_genglsl_impl.mtlx",
                        "start": 65650,
                        "end": 66087,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/genglsl/mx_backface_util.glsl",
                        "start": 66087,
                        "end": 66297,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/genmdl/adsklib_genmdl_impl.mtlx",
                        "start": 66297,
                        "end": 66714,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/genosl/adsklib_genosl_impl.mtlx",
                        "start": 66714,
                        "end": 67149,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/adsklib/genosl/mx_backface_util.osl",
                        "start": 67149,
                        "end": 67373,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/lights/README.md",
                        "start": 67373,
                        "end": 67373,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/materials/README.md",
                        "start": 67373,
                        "end": 67373,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/materials/wood/parquet_3/adsk_parquet_03_material.mtlx",
                        "start": 67373,
                        "end": 68451,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/procedurals/README.md",
                        "start": 68451,
                        "end": 68451,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/README.md",
                        "start": 68451,
                        "end": 68451,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/brick/adsk_brick_02_displacementshader.mtlx",
                        "start": 68451,
                        "end": 72383,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/brick/adsk_brick_02_surfaceshader.mtlx",
                        "start": 72383,
                        "end": 76315,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/brick/adsk_brick_10_displacementshader.mtlx",
                        "start": 76315,
                        "end": 78273,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/brick/adsk_brick_10_surfaceshader.mtlx",
                        "start": 78273,
                        "end": 80311,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/plastic/adsk_plastic08_red_surfaceshader.mtlx",
                        "start": 80311,
                        "end": 81162,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/wood/parquet_3/adsk_parquet_03_displacementshader.mtlx",
                        "start": 81162,
                        "end": 83139,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/shaders/wood/parquet_3/adsk_parquet_03_surfaceshader.mtlx",
                        "start": 83139,
                        "end": 85199,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/README.md",
                        "start": 85199,
                        "end": 85199,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_ao_2k_raw.exr",
                        "start": 85199,
                        "end": 178938,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_ao_2k_raw.png",
                        "start": 178938,
                        "end": 223804,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_basecolor_2k_acescg.exr",
                        "start": 223804,
                        "end": 526626,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_basecolor_2k_acescg.png",
                        "start": 526626,
                        "end": 649456,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_height_2k_raw.exr",
                        "start": 649456,
                        "end": 726779,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_normal_2k_raw.exr",
                        "start": 726779,
                        "end": 988848,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_normal_2k_raw.png",
                        "start": 988848,
                        "end": 1111624,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_roughness_2k_raw.exr",
                        "start": 1111624,
                        "end": 1199649,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_02/2k/bricks_02_roughness_2k_raw.png",
                        "start": 1199649,
                        "end": 1242209,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_ao_2k_raw.exr",
                        "start": 1242209,
                        "end": 1323442,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_ao_2k_raw.png",
                        "start": 1323442,
                        "end": 1351241,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_basecolor_2k_acescg.exr",
                        "start": 1351241,
                        "end": 1665438,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_basecolor_2k_acescg.png",
                        "start": 1665438,
                        "end": 1770644,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_height_2k_raw.exr",
                        "start": 1770644,
                        "end": 1854705,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_normal_2k_raw.exr",
                        "start": 1854705,
                        "end": 2104547,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_normal_2k_raw.png",
                        "start": 2104547,
                        "end": 2220175,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_roughness_2k_raw.exr",
                        "start": 2220175,
                        "end": 2308081,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/bricks_10/2k/bricks_10_roughness_2k_raw.png",
                        "start": 2308081,
                        "end": 2350151,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_ao_2k_raw.exr",
                        "start": 2350151,
                        "end": 2415634,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_ao_2k_raw.png",
                        "start": 2415634,
                        "end": 2422935,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_basecolor_2k_acescg.exr",
                        "start": 2422935,
                        "end": 2728365,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_basecolor_2k_acescg.png",
                        "start": 2728365,
                        "end": 2850803,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_height_2k_raw.exr",
                        "start": 2850803,
                        "end": 2904829,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_height_2k_raw.png",
                        "start": 2904829,
                        "end": 2913640,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_normal_2k_raw.exr",
                        "start": 2913640,
                        "end": 3156805,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_normal_2k_raw.png",
                        "start": 3156805,
                        "end": 3239640,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_roughness_2k_raw.exr",
                        "start": 3239640,
                        "end": 3375176,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/textures/parquet_03/2k/parquet_03_roughness_2k_raw.png",
                        "start": 3375176,
                        "end": 3420450,
                        "audio": 0
                    }, {
                        "filename": "/libraries/adsk/user/README.md",
                        "start": 3420450,
                        "end": 3420450,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/disney_brdf_2012.mtlx",
                        "start": 3420450,
                        "end": 3421507,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/disney_brdf_2015.mtlx",
                        "start": 3421507,
                        "end": 3422972,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/standard_surface.mtlx",
                        "start": 3422972,
                        "end": 3448576,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/usd_preview_surface.mtlx",
                        "start": 3448576,
                        "end": 3465562,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_add.mtlx",
                        "start": 3465562,
                        "end": 3468236,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_conductor.mtlx",
                        "start": 3468236,
                        "end": 3478133,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_dielectric.mtlx",
                        "start": 3478133,
                        "end": 3488087,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_diffuse.mtlx",
                        "start": 3488087,
                        "end": 3490672,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_emission.mtlx",
                        "start": 3490672,
                        "end": 3491441,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_layer.mtlx",
                        "start": 3491441,
                        "end": 3493198,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_mix.mtlx",
                        "start": 3493198,
                        "end": 3495256,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_sheen.mtlx",
                        "start": 3495256,
                        "end": 3497420,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_sss.mtlx",
                        "start": 3497420,
                        "end": 3504347,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/lama/lama_translucent.mtlx",
                        "start": 3504347,
                        "end": 3506425,
                        "audio": 0
                    }, {
                        "filename": "/libraries/bxdf/translation/standard_surface_to_usd.mtlx",
                        "start": 3506425,
                        "end": 3511413,
                        "audio": 0
                    }, {
                        "filename": "/libraries/lights/lights_defs.mtlx",
                        "start": 3511413,
                        "end": 3513906,
                        "audio": 0
                    }, {
                        "filename": "/libraries/lights/genglsl/lights_genglsl_impl.mtlx",
                        "start": 3513906,
                        "end": 3514582,
                        "audio": 0
                    }, {
                        "filename": "/libraries/lights/genglsl/mx_directional_light.glsl",
                        "start": 3514582,
                        "end": 3514768,
                        "audio": 0
                    }, {
                        "filename": "/libraries/lights/genglsl/mx_point_light.glsl",
                        "start": 3514768,
                        "end": 3515146,
                        "audio": 0
                    }, {
                        "filename": "/libraries/lights/genglsl/mx_spot_light.glsl",
                        "start": 3515146,
                        "end": 3515783,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/pbrlib_defs.mtlx",
                        "start": 3515783,
                        "end": 3539365,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/pbrlib_ng.mtlx",
                        "start": 3539365,
                        "end": 3546507,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_add_bsdf.glsl",
                        "start": 3546507,
                        "end": 3546859,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_add_edf.glsl",
                        "start": 3546859,
                        "end": 3546957,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_artistic_ior.glsl",
                        "start": 3546957,
                        "end": 3547547,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_burley_diffuse_bsdf.glsl",
                        "start": 3547547,
                        "end": 3548599,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_conductor_bsdf.glsl",
                        "start": 3548599,
                        "end": 3550896,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_dielectric_bsdf.glsl",
                        "start": 3550896,
                        "end": 3555033,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_generalized_schlick_bsdf.glsl",
                        "start": 3555033,
                        "end": 3558822,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_mix_bsdf.glsl",
                        "start": 3558822,
                        "end": 3559261,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_mix_edf.glsl",
                        "start": 3559261,
                        "end": 3559388,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_multiply_bsdf_color.glsl",
                        "start": 3559388,
                        "end": 3559824,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_multiply_bsdf_float.glsl",
                        "start": 3559824,
                        "end": 3560263,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_multiply_edf_color.glsl",
                        "start": 3560263,
                        "end": 3560373,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_multiply_edf_float.glsl",
                        "start": 3560373,
                        "end": 3560484,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_oren_nayar_diffuse_bsdf.glsl",
                        "start": 3560484,
                        "end": 3561453,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_roughness_anisotropy.glsl",
                        "start": 3561453,
                        "end": 3561913,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_roughness_dual.glsl",
                        "start": 3561913,
                        "end": 3562190,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_sheen_bsdf.glsl",
                        "start": 3562190,
                        "end": 3563810,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_subsurface_bsdf.glsl",
                        "start": 3563810,
                        "end": 3564880,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_translucent_bsdf.glsl",
                        "start": 3564880,
                        "end": 3565766,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/mx_uniform_edf.glsl",
                        "start": 3565766,
                        "end": 3565858,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/pbrlib_genglsl_impl.mtlx",
                        "start": 3565858,
                        "end": 3570354,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_environment_fis.glsl",
                        "start": 3570354,
                        "end": 3573226,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_environment_none.glsl",
                        "start": 3573226,
                        "end": 3573491,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_environment_prefilter.glsl",
                        "start": 3573491,
                        "end": 3574517,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_microfacet.glsl",
                        "start": 3574517,
                        "end": 3576917,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_microfacet_diffuse.glsl",
                        "start": 3576917,
                        "end": 3580159,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_microfacet_sheen.glsl",
                        "start": 3580159,
                        "end": 3583242,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_microfacet_specular.glsl",
                        "start": 3583242,
                        "end": 3597295,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_shadow.glsl",
                        "start": 3597295,
                        "end": 3598074,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/lib/mx_table.glsl",
                        "start": 3598074,
                        "end": 3598518,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/ogsfx/mx_lighting_functions.glsl",
                        "start": 3598518,
                        "end": 3602654,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genglsl/ogsfx/mx_lighting_uniforms.glsl",
                        "start": 3602654,
                        "end": 3614552,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genmdl/pbrlib_genmdl_impl.mtlx",
                        "start": 3614552,
                        "end": 3622284,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_add.inline",
                        "start": 3622284,
                        "end": 3622310,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_artistic_ior.osl",
                        "start": 3622310,
                        "end": 3622919,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_burley_diffuse_bsdf.osl",
                        "start": 3622919,
                        "end": 3623131,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_conductor_bsdf.osl",
                        "start": 3623131,
                        "end": 3624262,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_dielectric_bsdf.osl",
                        "start": 3624262,
                        "end": 3625688,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_displacement_float.osl",
                        "start": 3625688,
                        "end": 3625833,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_displacement_vector3.osl",
                        "start": 3625833,
                        "end": 3625973,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_generalized_schlick_bsdf.osl",
                        "start": 3625973,
                        "end": 3627377,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_mix.inline",
                        "start": 3627377,
                        "end": 3627407,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_multiply_bsdf.inline",
                        "start": 3627407,
                        "end": 3627443,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_multiply_edf.inline",
                        "start": 3627443,
                        "end": 3627462,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_oren_nayar_diffuse_bsdf.inline",
                        "start": 3627462,
                        "end": 3627526,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_roughness_anisotropy.osl",
                        "start": 3627526,
                        "end": 3627992,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_roughness_dual.osl",
                        "start": 3627992,
                        "end": 3628300,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_sheen_bsdf.osl",
                        "start": 3628300,
                        "end": 3629137,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_subsurface_bsdf.osl",
                        "start": 3629137,
                        "end": 3629385,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_surface.osl",
                        "start": 3629385,
                        "end": 3629612,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_translucent_bsdf.inline",
                        "start": 3629612,
                        "end": 3629662,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/mx_uniform_edf.inline",
                        "start": 3629662,
                        "end": 3629686,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/pbrlib_genosl_impl.mtlx",
                        "start": 3629686,
                        "end": 3634131,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/lib/mx_microfacet.osl",
                        "start": 3634131,
                        "end": 3637247,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/lib/mx_microfacet_sheen.osl",
                        "start": 3637247,
                        "end": 3637811,
                        "audio": 0
                    }, {
                        "filename": "/libraries/pbrlib/genosl/lib/mx_microfacet_specular.osl",
                        "start": 3637811,
                        "end": 3642159,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/stdlib_defs.mtlx",
                        "start": 3642159,
                        "end": 3866548,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/stdlib_ng.mtlx",
                        "start": 3866548,
                        "end": 3973890,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_aastep.glsl",
                        "start": 3973890,
                        "end": 3974095,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_absval.inline",
                        "start": 3974095,
                        "end": 3974108,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_acos.inline",
                        "start": 3974108,
                        "end": 3974122,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_add.inline",
                        "start": 3974122,
                        "end": 3974141,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_add_surfaceshader.glsl",
                        "start": 3974141,
                        "end": 3974387,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ap1_to_rec709_color3.glsl",
                        "start": 3974387,
                        "end": 3974544,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ap1_to_rec709_color4.glsl",
                        "start": 3974544,
                        "end": 3974718,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_asin.inline",
                        "start": 3974718,
                        "end": 3974732,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_atan2.inline",
                        "start": 3974732,
                        "end": 3974756,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_burn_color3.glsl",
                        "start": 3974756,
                        "end": 3975030,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_burn_color4.glsl",
                        "start": 3975030,
                        "end": 3975354,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_burn_float.glsl",
                        "start": 3975354,
                        "end": 3975586,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ceil.inline",
                        "start": 3975586,
                        "end": 3975600,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_cellnoise2d_float.glsl",
                        "start": 3975600,
                        "end": 3975758,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_cellnoise3d_float.glsl",
                        "start": 3975758,
                        "end": 3975916,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_clamp.inline",
                        "start": 3975916,
                        "end": 3975950,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_constant.inline",
                        "start": 3975950,
                        "end": 3975961,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_cos.inline",
                        "start": 3975961,
                        "end": 3975974,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_crossproduct.inline",
                        "start": 3975974,
                        "end": 3975999,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_determinant.inline",
                        "start": 3975999,
                        "end": 3976020,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_difference.inline",
                        "start": 3976020,
                        "end": 3976077,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_disjointover_color4.glsl",
                        "start": 3976077,
                        "end": 3976696,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_divide.inline",
                        "start": 3976696,
                        "end": 3976715,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_dodge.inline",
                        "start": 3976715,
                        "end": 3976773,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_dodge_color3.glsl",
                        "start": 3976773,
                        "end": 3977052,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_dodge_color4.glsl",
                        "start": 3977052,
                        "end": 3977384,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_dodge_float.glsl",
                        "start": 3977384,
                        "end": 3977615,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_dot.inline",
                        "start": 3977615,
                        "end": 3977623,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_dotproduct.inline",
                        "start": 3977623,
                        "end": 3977646,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_exp.inline",
                        "start": 3977646,
                        "end": 3977659,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_floor.inline",
                        "start": 3977659,
                        "end": 3977674,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_fa_vector2.glsl",
                        "start": 3977674,
                        "end": 3977971,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_fa_vector3.glsl",
                        "start": 3977971,
                        "end": 3978265,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_fa_vector4.glsl",
                        "start": 3978265,
                        "end": 3978663,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_float.glsl",
                        "start": 3978663,
                        "end": 3978955,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_vector2.glsl",
                        "start": 3978955,
                        "end": 3979248,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_vector3.glsl",
                        "start": 3979248,
                        "end": 3979538,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_fractal3d_vector4.glsl",
                        "start": 3979538,
                        "end": 3979938,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_g22_ap1_to_lin_rec709_color3.glsl",
                        "start": 3979938,
                        "end": 3980135,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_g22_ap1_to_lin_rec709_color4.glsl",
                        "start": 3980135,
                        "end": 3980349,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_gamma18_to_linear_color3.glsl",
                        "start": 3980349,
                        "end": 3980467,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_gamma18_to_linear_color4.glsl",
                        "start": 3980467,
                        "end": 3980625,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_gamma22_to_linear_color3.glsl",
                        "start": 3980625,
                        "end": 3980743,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_gamma22_to_linear_color4.glsl",
                        "start": 3980743,
                        "end": 3980901,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_gamma24_to_linear_color3.glsl",
                        "start": 3980901,
                        "end": 3981019,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_gamma24_to_linear_color4.glsl",
                        "start": 3981019,
                        "end": 3981177,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_hsvtorgb_color3.glsl",
                        "start": 3981177,
                        "end": 3981312,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_hsvtorgb_color4.glsl",
                        "start": 3981312,
                        "end": 3981462,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_image_color3.glsl",
                        "start": 3981462,
                        "end": 3981967,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_image_color4.glsl",
                        "start": 3981967,
                        "end": 3982468,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_image_float.glsl",
                        "start": 3982468,
                        "end": 3982972,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_image_vector2.glsl",
                        "start": 3982972,
                        "end": 3983477,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_image_vector3.glsl",
                        "start": 3983477,
                        "end": 3983983,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_image_vector4.glsl",
                        "start": 3983983,
                        "end": 3984485,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_inside.inline",
                        "start": 3984485,
                        "end": 3984504,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_invert.inline",
                        "start": 3984504,
                        "end": 3984525,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_invertM.inline",
                        "start": 3984525,
                        "end": 3984542,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_in_color4.inline",
                        "start": 3984542,
                        "end": 3984600,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ln.inline",
                        "start": 3984600,
                        "end": 3984613,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_luminance_color3.glsl",
                        "start": 3984613,
                        "end": 3984731,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_luminance_color4.glsl",
                        "start": 3984731,
                        "end": 3984866,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_magnitude.inline",
                        "start": 3984866,
                        "end": 3984882,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_mask_color4.inline",
                        "start": 3984882,
                        "end": 3984940,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_matte_color4.inline",
                        "start": 3984940,
                        "end": 3985073,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_max.inline",
                        "start": 3985073,
                        "end": 3985096,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_min.inline",
                        "start": 3985096,
                        "end": 3985119,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_minus.inline",
                        "start": 3985119,
                        "end": 3985173,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_mix.inline",
                        "start": 3985173,
                        "end": 3985203,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_mix_surfaceshader.glsl",
                        "start": 3985203,
                        "end": 3985442,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_modulo.inline",
                        "start": 3985442,
                        "end": 3985465,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_multiply.inline",
                        "start": 3985465,
                        "end": 3985484,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_multiply_surfaceshader_color3.glsl",
                        "start": 3985484,
                        "end": 3985700,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_multiply_surfaceshader_float.glsl",
                        "start": 3985700,
                        "end": 3985930,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_fa_vector2.glsl",
                        "start": 3985930,
                        "end": 3986167,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_fa_vector3.glsl",
                        "start": 3986167,
                        "end": 3986401,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_fa_vector4.glsl",
                        "start": 3986401,
                        "end": 3986703,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_float.glsl",
                        "start": 3986703,
                        "end": 3986935,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_vector2.glsl",
                        "start": 3986935,
                        "end": 3987168,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_vector3.glsl",
                        "start": 3987168,
                        "end": 3987398,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise2d_vector4.glsl",
                        "start": 3987398,
                        "end": 3987696,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_fa_vector2.glsl",
                        "start": 3987696,
                        "end": 3987933,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_fa_vector3.glsl",
                        "start": 3987933,
                        "end": 3988167,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_fa_vector4.glsl",
                        "start": 3988167,
                        "end": 3988473,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_float.glsl",
                        "start": 3988473,
                        "end": 3988705,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_vector2.glsl",
                        "start": 3988705,
                        "end": 3988938,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_vector3.glsl",
                        "start": 3988938,
                        "end": 3989168,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_noise3d_vector4.glsl",
                        "start": 3989168,
                        "end": 3989470,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_normalize.inline",
                        "start": 3989470,
                        "end": 3989489,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_normalmap.glsl",
                        "start": 3989489,
                        "end": 3989937,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_outside.inline",
                        "start": 3989937,
                        "end": 3989964,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_out_color2.inline",
                        "start": 3989964,
                        "end": 3990028,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_out_color4.inline",
                        "start": 3990028,
                        "end": 3990092,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_overlay.glsl",
                        "start": 3990092,
                        "end": 3990713,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_overlay_color3.glsl",
                        "start": 3990713,
                        "end": 3990891,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_overlay_color4.glsl",
                        "start": 3990891,
                        "end": 3991069,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_overlay_float.inline",
                        "start": 3991069,
                        "end": 3991209,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_over_color2.inline",
                        "start": 3991209,
                        "end": 3991279,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_over_color4.inline",
                        "start": 3991279,
                        "end": 3991351,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_plus.inline",
                        "start": 3991351,
                        "end": 3991405,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_power.inline",
                        "start": 3991405,
                        "end": 3991428,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_power_color3_float.inline",
                        "start": 3991428,
                        "end": 3991457,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_power_color4_float.inline",
                        "start": 3991457,
                        "end": 3991486,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_power_vector2_float.inline",
                        "start": 3991486,
                        "end": 3991515,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_power_vector3_float.inline",
                        "start": 3991515,
                        "end": 3991544,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_power_vector4_float.inline",
                        "start": 3991544,
                        "end": 3991573,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_premult_color4.glsl",
                        "start": 3991573,
                        "end": 3991674,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramplr_float.glsl",
                        "start": 3991674,
                        "end": 3991829,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramplr_vector2.glsl",
                        "start": 3991829,
                        "end": 3991983,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramplr_vector3.glsl",
                        "start": 3991983,
                        "end": 3992137,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramplr_vector4.glsl",
                        "start": 3992137,
                        "end": 3992291,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramptb_float.glsl",
                        "start": 3992291,
                        "end": 3992446,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramptb_vector2.glsl",
                        "start": 3992446,
                        "end": 3992600,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramptb_vector3.glsl",
                        "start": 3992600,
                        "end": 3992754,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_ramptb_vector4.glsl",
                        "start": 3992754,
                        "end": 3992908,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_remap.inline",
                        "start": 3992908,
                        "end": 3992999,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_rgbtohsv_color3.glsl",
                        "start": 3992999,
                        "end": 3993134,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_rgbtohsv_color4.glsl",
                        "start": 3993134,
                        "end": 3993284,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_rotate_vector2.glsl",
                        "start": 3993284,
                        "end": 3993540,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_rotate_vector3.glsl",
                        "start": 3993540,
                        "end": 3994451,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_screen.inline",
                        "start": 3994451,
                        "end": 3994529,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_sign.inline",
                        "start": 3994529,
                        "end": 3994543,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_sin.inline",
                        "start": 3994543,
                        "end": 3994556,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_float.glsl",
                        "start": 3994556,
                        "end": 3994790,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_vec2.glsl",
                        "start": 3994790,
                        "end": 3995039,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_vec2FA.glsl",
                        "start": 3995039,
                        "end": 3995284,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_vec3.glsl",
                        "start": 3995284,
                        "end": 3995591,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_vec3FA.glsl",
                        "start": 3995591,
                        "end": 3995890,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_vec4.glsl",
                        "start": 3995890,
                        "end": 3996255,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_smoothstep_vec4FA.glsl",
                        "start": 3996255,
                        "end": 3996608,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splitlr_float.glsl",
                        "start": 3996608,
                        "end": 3996822,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splitlr_vector2.glsl",
                        "start": 3996822,
                        "end": 3997035,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splitlr_vector3.glsl",
                        "start": 3997035,
                        "end": 3997248,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splitlr_vector4.glsl",
                        "start": 3997248,
                        "end": 3997461,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splittb_float.glsl",
                        "start": 3997461,
                        "end": 3997675,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splittb_vector2.glsl",
                        "start": 3997675,
                        "end": 3997888,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splittb_vector3.glsl",
                        "start": 3997888,
                        "end": 3998101,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_splittb_vector4.glsl",
                        "start": 3998101,
                        "end": 3998314,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_sqrt.inline",
                        "start": 3998314,
                        "end": 3998328,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_srgb_texture_to_lin_rec709_color3.glsl",
                        "start": 3998328,
                        "end": 3998511,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_srgb_texture_to_lin_rec709_color4.glsl",
                        "start": 3998511,
                        "end": 3998711,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_subtract.inline",
                        "start": 3998711,
                        "end": 3998730,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_tan.inline",
                        "start": 3998730,
                        "end": 3998743,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_transformmatrix.inline",
                        "start": 3998743,
                        "end": 3998761,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_transformmatrix_vector2M3.glsl",
                        "start": 3998761,
                        "end": 3998907,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_transformmatrix_vector3M4.glsl",
                        "start": 3998907,
                        "end": 3999054,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_transpose.inline",
                        "start": 3999054,
                        "end": 3999073,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/mx_unpremult_color4.glsl",
                        "start": 3999073,
                        "end": 3999176,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/stdlib_genglsl_cm_impl.mtlx",
                        "start": 3999176,
                        "end": 4001705,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/stdlib_genglsl_impl.mtlx",
                        "start": 4001705,
                        "end": 4079412,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/stdlib_genglsl_unit_impl.mtlx",
                        "start": 4079412,
                        "end": 4080311,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_hsv.glsl",
                        "start": 4080311,
                        "end": 4083880,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_math.glsl",
                        "start": 4083880,
                        "end": 4084055,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_noise.glsl",
                        "start": 4084055,
                        "end": 4095355,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_sampling.glsl",
                        "start": 4095355,
                        "end": 4098285,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_transform_color.glsl",
                        "start": 4098285,
                        "end": 4099149,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_transform_uv.glsl",
                        "start": 4099149,
                        "end": 4099270,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genglsl/lib/mx_transform_uv_vflip.glsl",
                        "start": 4099270,
                        "end": 4099411,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genmdl/stdlib_genmdl_cm_impl.mtlx",
                        "start": 4099411,
                        "end": 4101512,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genmdl/stdlib_genmdl_impl.mtlx",
                        "start": 4101512,
                        "end": 4189394,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genmdl/stdlib_genmdl_unit_impl.mtlx",
                        "start": 4189394,
                        "end": 4190277,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_absval.inline",
                        "start": 4190277,
                        "end": 4190290,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_acos.inline",
                        "start": 4190290,
                        "end": 4190304,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_add.inline",
                        "start": 4190304,
                        "end": 4190330,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ambientocclusion_float.osl",
                        "start": 4190330,
                        "end": 4190518,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ap1_to_rec709_color3.osl",
                        "start": 4190518,
                        "end": 4191021,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ap1_to_rec709_color4.osl",
                        "start": 4191021,
                        "end": 4191566,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_asin.inline",
                        "start": 4191566,
                        "end": 4191580,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_atan2.inline",
                        "start": 4191580,
                        "end": 4191604,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_bitangent_vector3.inline",
                        "start": 4191604,
                        "end": 4191632,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_burn_color3.osl",
                        "start": 4191632,
                        "end": 4191907,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_burn_color4.osl",
                        "start": 4191907,
                        "end": 4192268,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_burn_float.osl",
                        "start": 4192268,
                        "end": 4192494,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ceil.inline",
                        "start": 4192494,
                        "end": 4192508,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_cellnoise2d_float.osl",
                        "start": 4192508,
                        "end": 4192629,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_cellnoise3d_float.osl",
                        "start": 4192629,
                        "end": 4192735,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_clamp.inline",
                        "start": 4192735,
                        "end": 4192769,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_constant.inline",
                        "start": 4192769,
                        "end": 4192780,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_contrast.inline",
                        "start": 4192780,
                        "end": 4192821,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_cos.inline",
                        "start": 4192821,
                        "end": 4192834,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_crossproduct.inline",
                        "start": 4192834,
                        "end": 4192859,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_determinant.inline",
                        "start": 4192859,
                        "end": 4192880,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_difference.inline",
                        "start": 4192880,
                        "end": 4192937,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_disjointover_color4.osl",
                        "start": 4192937,
                        "end": 4193540,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_divide.inline",
                        "start": 4193540,
                        "end": 4193559,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_dodge_color3.osl",
                        "start": 4193559,
                        "end": 4193839,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_dodge_color4.osl",
                        "start": 4193839,
                        "end": 4194209,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_dodge_float.osl",
                        "start": 4194209,
                        "end": 4194425,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_dot.inline",
                        "start": 4194425,
                        "end": 4194433,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_dotproduct.inline",
                        "start": 4194433,
                        "end": 4194456,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_exp.inline",
                        "start": 4194456,
                        "end": 4194469,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_floor.inline",
                        "start": 4194469,
                        "end": 4194484,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_color3.osl",
                        "start": 4194484,
                        "end": 4194719,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_color4.osl",
                        "start": 4194719,
                        "end": 4195007,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_fa_color3.osl",
                        "start": 4195007,
                        "end": 4195244,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_fa_color4.osl",
                        "start": 4195244,
                        "end": 4195483,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_fa_vector2.osl",
                        "start": 4195483,
                        "end": 4195725,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_fa_vector3.osl",
                        "start": 4195725,
                        "end": 4195965,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_fa_vector4.osl",
                        "start": 4195965,
                        "end": 4196207,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_float.osl",
                        "start": 4196207,
                        "end": 4196440,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_vector2.osl",
                        "start": 4196440,
                        "end": 4196681,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_vector3.osl",
                        "start": 4196681,
                        "end": 4196919,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_fractal3d_vector4.osl",
                        "start": 4196919,
                        "end": 4197160,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_frame_float.osl",
                        "start": 4197160,
                        "end": 4197244,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_g22_ap1_to_lin_rec709_color3.osl",
                        "start": 4197244,
                        "end": 4197898,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_g22_ap1_to_lin_rec709_color4.osl",
                        "start": 4197898,
                        "end": 4198627,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_gamma18_to_linear_color3.osl",
                        "start": 4198627,
                        "end": 4198799,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_gamma18_to_linear_color4.osl",
                        "start": 4198799,
                        "end": 4198998,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_gamma22_to_linear_color3.osl",
                        "start": 4198998,
                        "end": 4199170,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_gamma22_to_linear_color4.osl",
                        "start": 4199170,
                        "end": 4199369,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_gamma24_to_linear_color3.osl",
                        "start": 4199369,
                        "end": 4199541,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_gamma24_to_linear_color4.osl",
                        "start": 4199541,
                        "end": 4199740,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geomcolor_color3.osl",
                        "start": 4199740,
                        "end": 4199840,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geomcolor_color4.osl",
                        "start": 4199840,
                        "end": 4200080,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geomcolor_float.osl",
                        "start": 4200080,
                        "end": 4200179,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_boolean.osl",
                        "start": 4200179,
                        "end": 4200335,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_color3.osl",
                        "start": 4200335,
                        "end": 4200493,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_color4.osl",
                        "start": 4200493,
                        "end": 4200877,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_float.osl",
                        "start": 4200877,
                        "end": 4201058,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_integer.osl",
                        "start": 4201058,
                        "end": 4201214,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_string.osl",
                        "start": 4201214,
                        "end": 4201375,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_vector2.osl",
                        "start": 4201375,
                        "end": 4201654,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_vector3.osl",
                        "start": 4201654,
                        "end": 4201815,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_geompropvalue_vector4.osl",
                        "start": 4201815,
                        "end": 4202148,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_heighttonormal_vector3.osl",
                        "start": 4202148,
                        "end": 4202326,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_hsvtorgb_color3.osl",
                        "start": 4202326,
                        "end": 4202435,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_hsvtorgb_color4.osl",
                        "start": 4202435,
                        "end": 4202561,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_image_color3.osl",
                        "start": 4202561,
                        "end": 4203285,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_image_color4.osl",
                        "start": 4203285,
                        "end": 4204186,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_image_float.osl",
                        "start": 4204186,
                        "end": 4204941,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_image_vector2.osl",
                        "start": 4204941,
                        "end": 4205749,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_image_vector3.osl",
                        "start": 4205749,
                        "end": 4206476,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_image_vector4.osl",
                        "start": 4206476,
                        "end": 4207439,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_in.inline",
                        "start": 4207439,
                        "end": 4207496,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_inside.inline",
                        "start": 4207496,
                        "end": 4207515,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_invert.inline",
                        "start": 4207515,
                        "end": 4207536,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_invertM.inline",
                        "start": 4207536,
                        "end": 4207548,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ln.inline",
                        "start": 4207548,
                        "end": 4207561,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_luminance_color3.osl",
                        "start": 4207561,
                        "end": 4207677,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_luminance_color4.osl",
                        "start": 4207677,
                        "end": 4207813,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_magnitude.inline",
                        "start": 4207813,
                        "end": 4207829,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_mask.inline",
                        "start": 4207829,
                        "end": 4207887,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_matte_color4.inline",
                        "start": 4207887,
                        "end": 4208020,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_max.inline",
                        "start": 4208020,
                        "end": 4208043,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_min.inline",
                        "start": 4208043,
                        "end": 4208066,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_minus.inline",
                        "start": 4208066,
                        "end": 4208120,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_mix.inline",
                        "start": 4208120,
                        "end": 4208150,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_modulo.inline",
                        "start": 4208150,
                        "end": 4208173,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_modulo_color3FA.inline",
                        "start": 4208173,
                        "end": 4208219,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_modulo_vector3FA.inline",
                        "start": 4208219,
                        "end": 4208266,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_multiply.inline",
                        "start": 4208266,
                        "end": 4208285,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_color3.osl",
                        "start": 4208285,
                        "end": 4208486,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_color4.osl",
                        "start": 4208486,
                        "end": 4208746,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_fa_color3.osl",
                        "start": 4208746,
                        "end": 4208949,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_fa_color4.osl",
                        "start": 4208949,
                        "end": 4209154,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_fa_vector2.osl",
                        "start": 4209154,
                        "end": 4209362,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_fa_vector3.osl",
                        "start": 4209362,
                        "end": 4209568,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_fa_vector4.osl",
                        "start": 4209568,
                        "end": 4209776,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_float.osl",
                        "start": 4209776,
                        "end": 4209975,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_vector2.osl",
                        "start": 4209975,
                        "end": 4210182,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_vector3.osl",
                        "start": 4210182,
                        "end": 4210386,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise2d_vector4.osl",
                        "start": 4210386,
                        "end": 4210593,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_color3.osl",
                        "start": 4210593,
                        "end": 4210779,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_color4.osl",
                        "start": 4210779,
                        "end": 4211018,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_fa_color3.osl",
                        "start": 4211018,
                        "end": 4211206,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_fa_color4.osl",
                        "start": 4211206,
                        "end": 4211396,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_fa_vector2.osl",
                        "start": 4211396,
                        "end": 4211589,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_fa_vector3.osl",
                        "start": 4211589,
                        "end": 4211780,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_fa_vector4.osl",
                        "start": 4211780,
                        "end": 4211973,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_float.osl",
                        "start": 4211973,
                        "end": 4212157,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_vector2.osl",
                        "start": 4212157,
                        "end": 4212349,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_vector3.osl",
                        "start": 4212349,
                        "end": 4212538,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_noise3d_vector4.osl",
                        "start": 4212538,
                        "end": 4212730,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_normalize.inline",
                        "start": 4212730,
                        "end": 4212749,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_normalmap.osl",
                        "start": 4212749,
                        "end": 4213277,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_normal_vector3.inline",
                        "start": 4213277,
                        "end": 4213302,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_out.inline",
                        "start": 4213302,
                        "end": 4213365,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_outside.inline",
                        "start": 4213365,
                        "end": 4213392,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_over.inline",
                        "start": 4213392,
                        "end": 4213463,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_overlay.inline",
                        "start": 4213463,
                        "end": 4213603,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_overlay_color3.osl",
                        "start": 4213603,
                        "end": 4214009,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_overlay_color4.osl",
                        "start": 4214009,
                        "end": 4214549,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_plus.inline",
                        "start": 4214549,
                        "end": 4214603,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_position_vector3.inline",
                        "start": 4214603,
                        "end": 4214628,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_power.inline",
                        "start": 4214628,
                        "end": 4214651,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_premult_color4.osl",
                        "start": 4214651,
                        "end": 4214757,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ramplr.inline",
                        "start": 4214757,
                        "end": 4214815,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_ramptb.inline",
                        "start": 4214815,
                        "end": 4214873,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_remap.inline",
                        "start": 4214873,
                        "end": 4214939,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_rgbtohsv_color3.osl",
                        "start": 4214939,
                        "end": 4215048,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_rgbtohsv_color4.osl",
                        "start": 4215048,
                        "end": 4215174,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_rotate_vector2.osl",
                        "start": 4215174,
                        "end": 4215442,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_rotate_vector3.osl",
                        "start": 4215442,
                        "end": 4216532,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_screen.inline",
                        "start": 4216532,
                        "end": 4216608,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_sign.inline",
                        "start": 4216608,
                        "end": 4216622,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_sin.inline",
                        "start": 4216622,
                        "end": 4216635,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_smoothstep.inline",
                        "start": 4216635,
                        "end": 4216672,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_splitlr.inline",
                        "start": 4216672,
                        "end": 4216737,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_splittb.inline",
                        "start": 4216737,
                        "end": 4216802,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_sqrt.inline",
                        "start": 4216802,
                        "end": 4216816,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_srgb_texture_to_lin_rec709_color3.osl",
                        "start": 4216816,
                        "end": 4217002,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_srgb_texture_to_lin_rec709_color4.osl",
                        "start": 4217002,
                        "end": 4217209,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_subtract.inline",
                        "start": 4217209,
                        "end": 4217235,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_tan.inline",
                        "start": 4217235,
                        "end": 4217248,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_tangent_vector3.inline",
                        "start": 4217248,
                        "end": 4217276,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_texcoord_vector2.inline",
                        "start": 4217276,
                        "end": 4217290,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_texcoord_vector3.inline",
                        "start": 4217290,
                        "end": 4217303,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_time_float.osl",
                        "start": 4217303,
                        "end": 4217445,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_transformmatrix.inline",
                        "start": 4217445,
                        "end": 4217473,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_transformmatrix_vector2M3.osl",
                        "start": 4217473,
                        "end": 4217665,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_transformnormal.inline",
                        "start": 4217665,
                        "end": 4217720,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_transformpoint.inline",
                        "start": 4217720,
                        "end": 4217774,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_transformvector.inline",
                        "start": 4217774,
                        "end": 4217821,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_transpose.inline",
                        "start": 4217821,
                        "end": 4217840,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/mx_unpremult_color4.osl",
                        "start": 4217840,
                        "end": 4217948,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/stdlib_genosl_cm_impl.mtlx",
                        "start": 4217948,
                        "end": 4220429,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/stdlib_genosl_impl.mtlx",
                        "start": 4220429,
                        "end": 4296624,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/stdlib_genosl_unit_impl.mtlx",
                        "start": 4296624,
                        "end": 4297507,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/lib/mx_sampling.osl",
                        "start": 4297507,
                        "end": 4303116,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/lib/mx_transform_color.osl",
                        "start": 4303116,
                        "end": 4303963,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/lib/mx_transform_uv.osl",
                        "start": 4303963,
                        "end": 4304034,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/genosl/lib/mx_transform_uv_vflip.osl",
                        "start": 4304034,
                        "end": 4304134,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/color4.h",
                        "start": 4304134,
                        "end": 4310257,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/matrix33.h",
                        "start": 4310257,
                        "end": 4313116,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_absval_color.osl",
                        "start": 4313116,
                        "end": 4313592,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_absval_color4.osl",
                        "start": 4313592,
                        "end": 4314103,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_absval_float.osl",
                        "start": 4314103,
                        "end": 4314579,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_absval_vector.osl",
                        "start": 4314579,
                        "end": 4315058,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_absval_vector2.osl",
                        "start": 4315058,
                        "end": 4315548,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_absval_vector4.osl",
                        "start": 4315548,
                        "end": 4316046,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_acos_color.osl",
                        "start": 4316046,
                        "end": 4316471,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_acos_color4.osl",
                        "start": 4316471,
                        "end": 4316932,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_acos_float.osl",
                        "start": 4316932,
                        "end": 4317357,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_acos_vector.osl",
                        "start": 4317357,
                        "end": 4317786,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_acos_vector2.osl",
                        "start": 4317786,
                        "end": 4318227,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_acos_vector4.osl",
                        "start": 4318227,
                        "end": 4318676,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_color.osl",
                        "start": 4318676,
                        "end": 4319152,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_color4.osl",
                        "start": 4319152,
                        "end": 4319681,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float.osl",
                        "start": 4319681,
                        "end": 4320157,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_color.osl",
                        "start": 4320157,
                        "end": 4320639,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_color4.osl",
                        "start": 4320639,
                        "end": 4321157,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_matrix33.osl",
                        "start": 4321157,
                        "end": 4321663,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_matrix44.osl",
                        "start": 4321663,
                        "end": 4322169,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_vector.osl",
                        "start": 4322169,
                        "end": 4322655,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_vector2.osl",
                        "start": 4322655,
                        "end": 4323153,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_float_vector4.osl",
                        "start": 4323153,
                        "end": 4323659,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_matrix33.osl",
                        "start": 4323659,
                        "end": 4324168,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_matrix44.osl",
                        "start": 4324168,
                        "end": 4324677,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_surfaceshader.osl",
                        "start": 4324677,
                        "end": 4325193,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_vector.osl",
                        "start": 4325193,
                        "end": 4325674,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_vector2.osl",
                        "start": 4325674,
                        "end": 4326172,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_add_vector4.osl",
                        "start": 4326172,
                        "end": 4326682,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ambientocclusion_float.osl",
                        "start": 4326682,
                        "end": 4327286,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_asin_color.osl",
                        "start": 4327286,
                        "end": 4327711,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_asin_color4.osl",
                        "start": 4327711,
                        "end": 4328172,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_asin_float.osl",
                        "start": 4328172,
                        "end": 4328597,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_asin_vector.osl",
                        "start": 4328597,
                        "end": 4329026,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_asin_vector2.osl",
                        "start": 4329026,
                        "end": 4329467,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_asin_vector4.osl",
                        "start": 4329467,
                        "end": 4329916,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_color.osl",
                        "start": 4329916,
                        "end": 4330379,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_color4.osl",
                        "start": 4330379,
                        "end": 4330895,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_float.osl",
                        "start": 4330895,
                        "end": 4331358,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_float_color.osl",
                        "start": 4331358,
                        "end": 4331829,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_float_color4.osl",
                        "start": 4331829,
                        "end": 4332336,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_float_vector.osl",
                        "start": 4332336,
                        "end": 4332811,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_float_vector2.osl",
                        "start": 4332811,
                        "end": 4333298,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_float_vector4.osl",
                        "start": 4333298,
                        "end": 4333793,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_vector.osl",
                        "start": 4333793,
                        "end": 4334261,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_vector2.osl",
                        "start": 4334261,
                        "end": 4334746,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_atan2_vector4.osl",
                        "start": 4334746,
                        "end": 4335243,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_bitangent_vector.osl",
                        "start": 4335243,
                        "end": 4335746,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_blur_color.osl",
                        "start": 4335746,
                        "end": 4336272,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_blur_color4.osl",
                        "start": 4336272,
                        "end": 4336833,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_blur_float.osl",
                        "start": 4336833,
                        "end": 4337359,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_blur_vector.osl",
                        "start": 4337359,
                        "end": 4337888,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_blur_vector2.osl",
                        "start": 4337888,
                        "end": 4338428,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_blur_vector4.osl",
                        "start": 4338428,
                        "end": 4338976,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_burn_color.osl",
                        "start": 4338976,
                        "end": 4339506,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_burn_color4.osl",
                        "start": 4339506,
                        "end": 4340089,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_burn_float.osl",
                        "start": 4340089,
                        "end": 4340619,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ceil_color.osl",
                        "start": 4340619,
                        "end": 4341099,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ceil_color4.osl",
                        "start": 4341099,
                        "end": 4341615,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ceil_float.osl",
                        "start": 4341615,
                        "end": 4342095,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ceil_vector.osl",
                        "start": 4342095,
                        "end": 4342579,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ceil_vector2.osl",
                        "start": 4342579,
                        "end": 4343075,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ceil_vector4.osl",
                        "start": 4343075,
                        "end": 4343579,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cellnoise2d_float.osl",
                        "start": 4343579,
                        "end": 4344151,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cellnoise3d_float.osl",
                        "start": 4344151,
                        "end": 4344752,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_color.osl",
                        "start": 4344752,
                        "end": 4345276,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_color4.osl",
                        "start": 4345276,
                        "end": 4345870,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_float.osl",
                        "start": 4345870,
                        "end": 4346394,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_float_color.osl",
                        "start": 4346394,
                        "end": 4346928,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_float_color4.osl",
                        "start": 4346928,
                        "end": 4347498,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_float_vector.osl",
                        "start": 4347498,
                        "end": 4348036,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_float_vector2.osl",
                        "start": 4348036,
                        "end": 4348586,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_float_vector4.osl",
                        "start": 4348586,
                        "end": 4349144,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_vector.osl",
                        "start": 4349144,
                        "end": 4349674,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_vector2.osl",
                        "start": 4349674,
                        "end": 4350226,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_clamp_vector4.osl",
                        "start": 4350226,
                        "end": 4350794,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine2_vector2.osl",
                        "start": 4350794,
                        "end": 4351452,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine3_color.osl",
                        "start": 4351452,
                        "end": 4352102,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine3_vector.osl",
                        "start": 4352102,
                        "end": 4352754,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine4_color4.osl",
                        "start": 4352754,
                        "end": 4353422,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine4_vector4.osl",
                        "start": 4353422,
                        "end": 4354084,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine_cf_color4.osl",
                        "start": 4354084,
                        "end": 4354711,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine_vf_vector4.osl",
                        "start": 4354711,
                        "end": 4355381,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_combine_vv_vector4.osl",
                        "start": 4355381,
                        "end": 4356061,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_compare_color.osl",
                        "start": 4356061,
                        "end": 4356895,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_compare_color4.osl",
                        "start": 4356895,
                        "end": 4357781,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_compare_float.osl",
                        "start": 4357781,
                        "end": 4358615,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_compare_vector.osl",
                        "start": 4358615,
                        "end": 4359453,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_compare_vector2.osl",
                        "start": 4359453,
                        "end": 4360307,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_compare_vector4.osl",
                        "start": 4360307,
                        "end": 4361173,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_bool.osl",
                        "start": 4361173,
                        "end": 4361594,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_color.osl",
                        "start": 4361594,
                        "end": 4362020,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_color4.osl",
                        "start": 4362020,
                        "end": 4362481,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_filename.osl",
                        "start": 4362481,
                        "end": 4362928,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_float.osl",
                        "start": 4362928,
                        "end": 4363354,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_int.osl",
                        "start": 4363354,
                        "end": 4363774,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_matrix33.osl",
                        "start": 4363774,
                        "end": 4364221,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_matrix44.osl",
                        "start": 4364221,
                        "end": 4364668,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_string.osl",
                        "start": 4364668,
                        "end": 4365113,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_vector.osl",
                        "start": 4365113,
                        "end": 4365542,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_vector2.osl",
                        "start": 4365542,
                        "end": 4365982,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_constant_vector4.osl",
                        "start": 4365982,
                        "end": 4366430,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cos_color.osl",
                        "start": 4366430,
                        "end": 4366852,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cos_color4.osl",
                        "start": 4366852,
                        "end": 4367310,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cos_float.osl",
                        "start": 4367310,
                        "end": 4367732,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cos_vector.osl",
                        "start": 4367732,
                        "end": 4368158,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cos_vector2.osl",
                        "start": 4368158,
                        "end": 4368596,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_cos_vector4.osl",
                        "start": 4368596,
                        "end": 4369042,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_crossproduct_vector.osl",
                        "start": 4369042,
                        "end": 4369530,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_determinant_matrix33.osl",
                        "start": 4369530,
                        "end": 4370004,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_determinant_matrix44.osl",
                        "start": 4370004,
                        "end": 4370478,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_disjointover_color4.osl",
                        "start": 4370478,
                        "end": 4371464,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_color.osl",
                        "start": 4371464,
                        "end": 4371949,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_color4.osl",
                        "start": 4371949,
                        "end": 4372487,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float.osl",
                        "start": 4372487,
                        "end": 4372972,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_color.osl",
                        "start": 4372972,
                        "end": 4373470,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_color4.osl",
                        "start": 4373470,
                        "end": 4374004,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_matrix33.osl",
                        "start": 4374004,
                        "end": 4374526,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_matrix44.osl",
                        "start": 4374526,
                        "end": 4375048,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_vector.osl",
                        "start": 4375048,
                        "end": 4375550,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_vector2.osl",
                        "start": 4375550,
                        "end": 4376064,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_float_vector4.osl",
                        "start": 4376064,
                        "end": 4376586,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_matrix33.osl",
                        "start": 4376586,
                        "end": 4377137,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_matrix44.osl",
                        "start": 4377137,
                        "end": 4377688,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_vector.osl",
                        "start": 4377688,
                        "end": 4378178,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_vector2.osl",
                        "start": 4378178,
                        "end": 4378685,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_divide_vector4.osl",
                        "start": 4378685,
                        "end": 4379204,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dodge_color.osl",
                        "start": 4379204,
                        "end": 4379727,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dodge_color4.osl",
                        "start": 4379727,
                        "end": 4380303,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dodge_float.osl",
                        "start": 4380303,
                        "end": 4380826,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dotproduct_vector.osl",
                        "start": 4380826,
                        "end": 4381328,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dotproduct_vector2.osl",
                        "start": 4381328,
                        "end": 4381842,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dotproduct_vector4.osl",
                        "start": 4381842,
                        "end": 4382364,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_bool.osl",
                        "start": 4382364,
                        "end": 4382775,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_color.osl",
                        "start": 4382775,
                        "end": 4383192,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_color4.osl",
                        "start": 4383192,
                        "end": 4383645,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_displacementshader.osl",
                        "start": 4383645,
                        "end": 4384113,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_filename.osl",
                        "start": 4384113,
                        "end": 4384554,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_float.osl",
                        "start": 4384554,
                        "end": 4384971,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_int.osl",
                        "start": 4384971,
                        "end": 4385380,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_lightshader.osl",
                        "start": 4385380,
                        "end": 4385811,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_matrix33.osl",
                        "start": 4385811,
                        "end": 4386252,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_matrix44.osl",
                        "start": 4386252,
                        "end": 4386693,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_string.osl",
                        "start": 4386693,
                        "end": 4387130,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_surfaceshader.osl",
                        "start": 4387130,
                        "end": 4387579,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_vector.osl",
                        "start": 4387579,
                        "end": 4388e3,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_vector2.osl",
                        "start": 4388e3,
                        "end": 4388433,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_vector4.osl",
                        "start": 4388433,
                        "end": 4388874,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_dot_volumeshader.osl",
                        "start": 4388874,
                        "end": 4389320,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exponent_float.osl",
                        "start": 4389320,
                        "end": 4389842,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exponent_vector.osl",
                        "start": 4389842,
                        "end": 4390369,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exponent_vector2.osl",
                        "start": 4390369,
                        "end": 4390913,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exponent_vector4.osl",
                        "start": 4390913,
                        "end": 4391469,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exp_color.osl",
                        "start": 4391469,
                        "end": 4391886,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exp_color4.osl",
                        "start": 4391886,
                        "end": 4392339,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exp_float.osl",
                        "start": 4392339,
                        "end": 4392756,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exp_vector.osl",
                        "start": 4392756,
                        "end": 4393177,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exp_vector2.osl",
                        "start": 4393177,
                        "end": 4393610,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_exp_vector4.osl",
                        "start": 4393610,
                        "end": 4394051,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_extract_color.osl",
                        "start": 4394051,
                        "end": 4394552,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_extract_color4.osl",
                        "start": 4394552,
                        "end": 4395071,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_extract_vector.osl",
                        "start": 4395071,
                        "end": 4395574,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_extract_vector2.osl",
                        "start": 4395574,
                        "end": 4396083,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_extract_vector4.osl",
                        "start": 4396083,
                        "end": 4396596,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_floor_color.osl",
                        "start": 4396596,
                        "end": 4397077,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_floor_color4.osl",
                        "start": 4397077,
                        "end": 4397594,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_floor_float.osl",
                        "start": 4397594,
                        "end": 4398075,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_floor_vector.osl",
                        "start": 4398075,
                        "end": 4398560,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_floor_vector2.osl",
                        "start": 4398560,
                        "end": 4399057,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_floor_vector4.osl",
                        "start": 4399057,
                        "end": 4399562,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_color.osl",
                        "start": 4399562,
                        "end": 4400693,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_color4.osl",
                        "start": 4400693,
                        "end": 4401860,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_fa_color.osl",
                        "start": 4401860,
                        "end": 4403002,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_fa_color4.osl",
                        "start": 4403002,
                        "end": 4404163,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_fa_vector.osl",
                        "start": 4404163,
                        "end": 4405308,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_fa_vector2.osl",
                        "start": 4405308,
                        "end": 4406460,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_fa_vector4.osl",
                        "start": 4406460,
                        "end": 4407616,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_float.osl",
                        "start": 4407616,
                        "end": 4408747,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_vector.osl",
                        "start": 4408747,
                        "end": 4409882,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_vector2.osl",
                        "start": 4409882,
                        "end": 4411029,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_fractal3d_vector4.osl",
                        "start": 4411029,
                        "end": 4412184,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_frame_float.osl",
                        "start": 4412184,
                        "end": 4412647,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_funcs.h",
                        "start": 4412647,
                        "end": 4435771,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geomcolor_color.osl",
                        "start": 4435771,
                        "end": 4436225,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geomcolor_color4.osl",
                        "start": 4436225,
                        "end": 4436697,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geomcolor_float.osl",
                        "start": 4436697,
                        "end": 4437151,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_bool.osl",
                        "start": 4437151,
                        "end": 4437690,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_color.osl",
                        "start": 4437690,
                        "end": 4438234,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_color4.osl",
                        "start": 4438234,
                        "end": 4438813,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_float.osl",
                        "start": 4438813,
                        "end": 4439357,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_int.osl",
                        "start": 4439357,
                        "end": 4439895,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_string.osl",
                        "start": 4439895,
                        "end": 4440452,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_vector.osl",
                        "start": 4440452,
                        "end": 4440999,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_vector2.osl",
                        "start": 4440999,
                        "end": 4441557,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_geompropvalue_vector4.osl",
                        "start": 4441557,
                        "end": 4442123,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_heighttonormal_vector.osl",
                        "start": 4442123,
                        "end": 4442687,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_hsvadjust_color.osl",
                        "start": 4442687,
                        "end": 4443808,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_hsvadjust_color4.osl",
                        "start": 4443808,
                        "end": 4444964,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_hsvtorgb_color.osl",
                        "start": 4444964,
                        "end": 4445468,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_hsvtorgb_color4.osl",
                        "start": 4445468,
                        "end": 4446006,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_hueshift_color.osl",
                        "start": 4446006,
                        "end": 4446982,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_hueshift_color4.osl",
                        "start": 4446982,
                        "end": 4447993,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_image_color.osl",
                        "start": 4447993,
                        "end": 4449721,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_image_color4.osl",
                        "start": 4449721,
                        "end": 4451484,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_image_float.osl",
                        "start": 4451484,
                        "end": 4453212,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_image_vector.osl",
                        "start": 4453212,
                        "end": 4454943,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_image_vector2.osl",
                        "start": 4454943,
                        "end": 4456685,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_image_vector4.osl",
                        "start": 4456685,
                        "end": 4458435,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_inside_color.osl",
                        "start": 4458435,
                        "end": 4459018,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_inside_color4.osl",
                        "start": 4459018,
                        "end": 4459637,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_inside_float.osl",
                        "start": 4459637,
                        "end": 4460220,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_color.osl",
                        "start": 4460220,
                        "end": 4460734,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_color4.osl",
                        "start": 4460734,
                        "end": 4461301,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_float.osl",
                        "start": 4461301,
                        "end": 4461815,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_float_color.osl",
                        "start": 4461815,
                        "end": 4462337,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_float_color4.osl",
                        "start": 4462337,
                        "end": 4462895,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_float_vector.osl",
                        "start": 4462895,
                        "end": 4463421,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_float_vector2.osl",
                        "start": 4463421,
                        "end": 4463959,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_float_vector4.osl",
                        "start": 4463959,
                        "end": 4464505,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_vector.osl",
                        "start": 4464505,
                        "end": 4465024,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_vector2.osl",
                        "start": 4465024,
                        "end": 4465560,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_invert_vector4.osl",
                        "start": 4465560,
                        "end": 4466108,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_in_color4.osl",
                        "start": 4466108,
                        "end": 4466748,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ln_color.osl",
                        "start": 4466748,
                        "end": 4467168,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ln_color4.osl",
                        "start": 4467168,
                        "end": 4467624,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ln_float.osl",
                        "start": 4467624,
                        "end": 4468044,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ln_vector.osl",
                        "start": 4468044,
                        "end": 4468468,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ln_vector2.osl",
                        "start": 4468468,
                        "end": 4468904,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ln_vector4.osl",
                        "start": 4468904,
                        "end": 4469348,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_luminance_color.osl",
                        "start": 4469348,
                        "end": 4470255,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_luminance_color4.osl",
                        "start": 4470255,
                        "end": 4471197,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_magnitude_vector.osl",
                        "start": 4471197,
                        "end": 4471698,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_magnitude_vector2.osl",
                        "start": 4471698,
                        "end": 4472206,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_magnitude_vector4.osl",
                        "start": 4472206,
                        "end": 4472718,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mask_color4.osl",
                        "start": 4472718,
                        "end": 4473360,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_matrix_invert_matrix33.osl",
                        "start": 4473360,
                        "end": 4473815,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_matrix_invert_matrix44.osl",
                        "start": 4473815,
                        "end": 4474270,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_matte_color4.osl",
                        "start": 4474270,
                        "end": 4475086,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_color.osl",
                        "start": 4475086,
                        "end": 4475571,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_color4.osl",
                        "start": 4475571,
                        "end": 4476109,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_float.osl",
                        "start": 4476109,
                        "end": 4476594,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_float_color.osl",
                        "start": 4476594,
                        "end": 4477086,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_float_color4.osl",
                        "start": 4477086,
                        "end": 4477614,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_float_vector.osl",
                        "start": 4477614,
                        "end": 4478110,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_float_vector2.osl",
                        "start": 4478110,
                        "end": 4478618,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_float_vector4.osl",
                        "start": 4478618,
                        "end": 4479134,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_vector.osl",
                        "start": 4479134,
                        "end": 4479624,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_vector2.osl",
                        "start": 4479624,
                        "end": 4480131,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_max_vector4.osl",
                        "start": 4480131,
                        "end": 4480650,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_color.osl",
                        "start": 4480650,
                        "end": 4481134,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_color4.osl",
                        "start": 4481134,
                        "end": 4481671,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_float.osl",
                        "start": 4481671,
                        "end": 4482155,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_float_color.osl",
                        "start": 4482155,
                        "end": 4482647,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_float_color4.osl",
                        "start": 4482647,
                        "end": 4483175,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_float_vector.osl",
                        "start": 4483175,
                        "end": 4483671,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_float_vector2.osl",
                        "start": 4483671,
                        "end": 4484179,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_float_vector4.osl",
                        "start": 4484179,
                        "end": 4484695,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_vector.osl",
                        "start": 4484695,
                        "end": 4485184,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_vector2.osl",
                        "start": 4485184,
                        "end": 4485690,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_min_vector4.osl",
                        "start": 4485690,
                        "end": 4486208,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_color.osl",
                        "start": 4486208,
                        "end": 4486739,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_color4.osl",
                        "start": 4486739,
                        "end": 4487323,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_float.osl",
                        "start": 4487323,
                        "end": 4487854,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_surfaceshader.osl",
                        "start": 4487854,
                        "end": 4488425,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_vector.osl",
                        "start": 4488425,
                        "end": 4488961,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_vector2.osl",
                        "start": 4488961,
                        "end": 4489514,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mix_vector4.osl",
                        "start": 4489514,
                        "end": 4490079,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_color.osl",
                        "start": 4490079,
                        "end": 4490733,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_color4.osl",
                        "start": 4490733,
                        "end": 4491440,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_float.osl",
                        "start": 4491440,
                        "end": 4492094,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_float_color.osl",
                        "start": 4492094,
                        "end": 4492756,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_float_color4.osl",
                        "start": 4492756,
                        "end": 4493454,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_float_vector.osl",
                        "start": 4493454,
                        "end": 4494120,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_float_vector2.osl",
                        "start": 4494120,
                        "end": 4494798,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_float_vector4.osl",
                        "start": 4494798,
                        "end": 4495484,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_vector.osl",
                        "start": 4495484,
                        "end": 4496143,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_vector2.osl",
                        "start": 4496143,
                        "end": 4496819,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_modulo_vector4.osl",
                        "start": 4496819,
                        "end": 4497507,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_color.osl",
                        "start": 4497507,
                        "end": 4497969,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_color4.osl",
                        "start": 4497969,
                        "end": 4498484,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float.osl",
                        "start": 4498484,
                        "end": 4498946,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_color.osl",
                        "start": 4498946,
                        "end": 4499417,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_color4.osl",
                        "start": 4499417,
                        "end": 4499924,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_matrix33.osl",
                        "start": 4499924,
                        "end": 4500419,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_matrix44.osl",
                        "start": 4500419,
                        "end": 4500914,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_vector.osl",
                        "start": 4500914,
                        "end": 4501389,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_vector2.osl",
                        "start": 4501389,
                        "end": 4501876,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_float_vector4.osl",
                        "start": 4501876,
                        "end": 4502371,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_matrix33.osl",
                        "start": 4502371,
                        "end": 4502899,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_matrix44.osl",
                        "start": 4502899,
                        "end": 4503427,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_vector.osl",
                        "start": 4503427,
                        "end": 4503894,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_vector2.osl",
                        "start": 4503894,
                        "end": 4504378,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_multiply_vector4.osl",
                        "start": 4504378,
                        "end": 4504874,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mult_surfaceshader_color.osl",
                        "start": 4504874,
                        "end": 4505367,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_mult_surfaceshader_float.osl",
                        "start": 4505367,
                        "end": 4505860,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_color.osl",
                        "start": 4505860,
                        "end": 4506693,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_color4.osl",
                        "start": 4506693,
                        "end": 4507561,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_fa_color.osl",
                        "start": 4507561,
                        "end": 4508435,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_fa_color4.osl",
                        "start": 4508435,
                        "end": 4509328,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_fa_vector.osl",
                        "start": 4509328,
                        "end": 4510205,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_fa_vector2.osl",
                        "start": 4510205,
                        "end": 4511089,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_fa_vector4.osl",
                        "start": 4511089,
                        "end": 4511977,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_float.osl",
                        "start": 4511977,
                        "end": 4512810,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_vector.osl",
                        "start": 4512810,
                        "end": 4513646,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_vector2.osl",
                        "start": 4513646,
                        "end": 4514493,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise2d_vector4.osl",
                        "start": 4514493,
                        "end": 4515348,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_color.osl",
                        "start": 4515348,
                        "end": 4516215,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_color4.osl",
                        "start": 4516215,
                        "end": 4517117,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_fa_color.osl",
                        "start": 4517117,
                        "end": 4518029,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_fa_color4.osl",
                        "start": 4518029,
                        "end": 4518960,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_fa_vector.osl",
                        "start": 4518960,
                        "end": 4519875,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_fa_vector2.osl",
                        "start": 4519875,
                        "end": 4520797,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_fa_vector4.osl",
                        "start": 4520797,
                        "end": 4521723,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_float.osl",
                        "start": 4521723,
                        "end": 4522590,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_vector.osl",
                        "start": 4522590,
                        "end": 4523460,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_vector2.osl",
                        "start": 4523460,
                        "end": 4524341,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_noise3d_vector4.osl",
                        "start": 4524341,
                        "end": 4525230,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_normalize_vector.osl",
                        "start": 4525230,
                        "end": 4525724,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_normalize_vector2.osl",
                        "start": 4525724,
                        "end": 4526230,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_normalize_vector4.osl",
                        "start": 4526230,
                        "end": 4526744,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_normal_vector.osl",
                        "start": 4526744,
                        "end": 4527210,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_outside_color.osl",
                        "start": 4527210,
                        "end": 4527808,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_outside_color4.osl",
                        "start": 4527808,
                        "end": 4528442,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_outside_float.osl",
                        "start": 4528442,
                        "end": 4529040,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_out_color4.osl",
                        "start": 4529040,
                        "end": 4529695,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_overlay_color.osl",
                        "start": 4529695,
                        "end": 4530583,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_overlay_color4.osl",
                        "start": 4530583,
                        "end": 4531524,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_overlay_float.osl",
                        "start": 4531524,
                        "end": 4532412,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_over_color4.osl",
                        "start": 4532412,
                        "end": 4533077,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_position_vector.osl",
                        "start": 4533077,
                        "end": 4533535,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_color.osl",
                        "start": 4533535,
                        "end": 4534049,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_color4.osl",
                        "start": 4534049,
                        "end": 4534616,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_float.osl",
                        "start": 4534616,
                        "end": 4535130,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_float_color.osl",
                        "start": 4535130,
                        "end": 4535652,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_float_color4.osl",
                        "start": 4535652,
                        "end": 4536210,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_float_vector.osl",
                        "start": 4536210,
                        "end": 4536736,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_float_vector2.osl",
                        "start": 4536736,
                        "end": 4537274,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_float_vector4.osl",
                        "start": 4537274,
                        "end": 4537820,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_vector.osl",
                        "start": 4537820,
                        "end": 4538339,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_vector2.osl",
                        "start": 4538339,
                        "end": 4538875,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_power_vector4.osl",
                        "start": 4538875,
                        "end": 4539423,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_premult_color.osl",
                        "start": 4539423,
                        "end": 4540216,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_premult_color4.osl",
                        "start": 4540216,
                        "end": 4541045,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramp4_color.osl",
                        "start": 4541045,
                        "end": 4542163,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramp4_color4.osl",
                        "start": 4542163,
                        "end": 4543367,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramp4_float.osl",
                        "start": 4543367,
                        "end": 4544485,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramp4_vector.osl",
                        "start": 4544485,
                        "end": 4545609,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramp4_vector2.osl",
                        "start": 4545609,
                        "end": 4546759,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramp4_vector4.osl",
                        "start": 4546759,
                        "end": 4547929,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramplr_color.osl",
                        "start": 4547929,
                        "end": 4548710,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramplr_color4.osl",
                        "start": 4548710,
                        "end": 4549543,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramplr_float.osl",
                        "start": 4549543,
                        "end": 4550324,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramplr_vector.osl",
                        "start": 4550324,
                        "end": 4551109,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramplr_vector2.osl",
                        "start": 4551109,
                        "end": 4551910,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramplr_vector4.osl",
                        "start": 4551910,
                        "end": 4552723,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramptb_color.osl",
                        "start": 4552723,
                        "end": 4553486,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramptb_color4.osl",
                        "start": 4553486,
                        "end": 4554301,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramptb_float.osl",
                        "start": 4554301,
                        "end": 4555064,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramptb_vector.osl",
                        "start": 4555064,
                        "end": 4555831,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramptb_vector2.osl",
                        "start": 4555831,
                        "end": 4556614,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_ramptb_vector4.osl",
                        "start": 4556614,
                        "end": 4557409,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_color.osl",
                        "start": 4557409,
                        "end": 4558515,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_color4.osl",
                        "start": 4558515,
                        "end": 4559707,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_float.osl",
                        "start": 4559707,
                        "end": 4560813,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_float_color.osl",
                        "start": 4560813,
                        "end": 4561681,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_float_color4.osl",
                        "start": 4561681,
                        "end": 4562585,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_float_vector.osl",
                        "start": 4562585,
                        "end": 4563457,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_float_vector2.osl",
                        "start": 4563457,
                        "end": 4564341,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_float_vector4.osl",
                        "start": 4564341,
                        "end": 4565233,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_vector.osl",
                        "start": 4565233,
                        "end": 4566345,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_vector2.osl",
                        "start": 4566345,
                        "end": 4567483,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_remap_vector4.osl",
                        "start": 4567483,
                        "end": 4568641,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_rgbtohsv_color.osl",
                        "start": 4568641,
                        "end": 4569145,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_rgbtohsv_color4.osl",
                        "start": 4569145,
                        "end": 4569683,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_rotate2d_vector2.osl",
                        "start": 4569683,
                        "end": 4570310,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_rotate_vector.osl",
                        "start": 4570310,
                        "end": 4571010,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_rotate_vector2.osl",
                        "start": 4571010,
                        "end": 4571721,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_scale_vector.osl",
                        "start": 4571721,
                        "end": 4572300,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_scale_vector2.osl",
                        "start": 4572300,
                        "end": 4572903,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_screen_color.osl",
                        "start": 4572903,
                        "end": 4573443,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_screen_color4.osl",
                        "start": 4573443,
                        "end": 4574036,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_screen_float.osl",
                        "start": 4574036,
                        "end": 4574576,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_separate_color.osl",
                        "start": 4574576,
                        "end": 4576143,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_separate_color4.osl",
                        "start": 4576143,
                        "end": 4577728,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_separate_vector.osl",
                        "start": 4577728,
                        "end": 4579297,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_separate_vector2.osl",
                        "start": 4579297,
                        "end": 4580872,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_separate_vector4.osl",
                        "start": 4580872,
                        "end": 4582451,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sign_color.osl",
                        "start": 4582451,
                        "end": 4582876,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sign_color4.osl",
                        "start": 4582876,
                        "end": 4583337,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sign_float.osl",
                        "start": 4583337,
                        "end": 4583762,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sign_vector.osl",
                        "start": 4583762,
                        "end": 4584191,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sign_vector2.osl",
                        "start": 4584191,
                        "end": 4584632,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sign_vector4.osl",
                        "start": 4584632,
                        "end": 4585081,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sin_color.osl",
                        "start": 4585081,
                        "end": 4585503,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sin_color4.osl",
                        "start": 4585503,
                        "end": 4585961,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sin_float.osl",
                        "start": 4585961,
                        "end": 4586383,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sin_vector.osl",
                        "start": 4586383,
                        "end": 4586809,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sin_vector2.osl",
                        "start": 4586809,
                        "end": 4587247,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sin_vector4.osl",
                        "start": 4587247,
                        "end": 4587693,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_color.osl",
                        "start": 4587693,
                        "end": 4588479,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_color4.osl",
                        "start": 4588479,
                        "end": 4589334,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_float.osl",
                        "start": 4589334,
                        "end": 4590120,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_float_color.osl",
                        "start": 4590120,
                        "end": 4590912,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_float_color4.osl",
                        "start": 4590912,
                        "end": 4591739,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_float_vector.osl",
                        "start": 4591739,
                        "end": 4592534,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_float_vector2.osl",
                        "start": 4592534,
                        "end": 4593340,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_float_vector4.osl",
                        "start": 4593340,
                        "end": 4594154,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_vector.osl",
                        "start": 4594154,
                        "end": 4594945,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_vector2.osl",
                        "start": 4594945,
                        "end": 4595757,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_smoothstep_vector4.osl",
                        "start": 4595757,
                        "end": 4596585,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splitlr_color.osl",
                        "start": 4596585,
                        "end": 4597408,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splitlr_color4.osl",
                        "start": 4597408,
                        "end": 4598283,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splitlr_float.osl",
                        "start": 4598283,
                        "end": 4599106,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splitlr_vector.osl",
                        "start": 4599106,
                        "end": 4599933,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splitlr_vector2.osl",
                        "start": 4599933,
                        "end": 4600776,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splitlr_vector4.osl",
                        "start": 4600776,
                        "end": 4601631,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splittb_color.osl",
                        "start": 4601631,
                        "end": 4602442,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splittb_color4.osl",
                        "start": 4602442,
                        "end": 4603305,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splittb_float.osl",
                        "start": 4603305,
                        "end": 4604116,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splittb_vector.osl",
                        "start": 4604116,
                        "end": 4604931,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splittb_vector2.osl",
                        "start": 4604931,
                        "end": 4605762,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_splittb_vector4.osl",
                        "start": 4605762,
                        "end": 4606605,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sqrt_color.osl",
                        "start": 4606605,
                        "end": 4607030,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sqrt_color4.osl",
                        "start": 4607030,
                        "end": 4607491,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sqrt_float.osl",
                        "start": 4607491,
                        "end": 4607916,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sqrt_vector.osl",
                        "start": 4607916,
                        "end": 4608345,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sqrt_vector2.osl",
                        "start": 4608345,
                        "end": 4608786,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_sqrt_vector4.osl",
                        "start": 4608786,
                        "end": 4609235,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_color.osl",
                        "start": 4609235,
                        "end": 4609723,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_color4.osl",
                        "start": 4609723,
                        "end": 4610264,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float.osl",
                        "start": 4610264,
                        "end": 4610752,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_color.osl",
                        "start": 4610752,
                        "end": 4611245,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_color4.osl",
                        "start": 4611245,
                        "end": 4611774,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_matrix33.osl",
                        "start": 4611774,
                        "end": 4612291,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_matrix44.osl",
                        "start": 4612291,
                        "end": 4612808,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_vector.osl",
                        "start": 4612808,
                        "end": 4613305,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_vector2.osl",
                        "start": 4613305,
                        "end": 4613814,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_float_vector4.osl",
                        "start": 4613814,
                        "end": 4614331,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_matrix33.osl",
                        "start": 4614331,
                        "end": 4614852,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_matrix44.osl",
                        "start": 4614852,
                        "end": 4615373,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_vector.osl",
                        "start": 4615373,
                        "end": 4615866,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_vector2.osl",
                        "start": 4615866,
                        "end": 4616376,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_subtract_vector4.osl",
                        "start": 4616376,
                        "end": 4616898,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_switch_color.osl",
                        "start": 4616898,
                        "end": 4617810,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_switch_color4.osl",
                        "start": 4617810,
                        "end": 4618825,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_switch_float.osl",
                        "start": 4618825,
                        "end": 4619737,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_switch_vector.osl",
                        "start": 4619737,
                        "end": 4620656,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_switch_vector2.osl",
                        "start": 4620656,
                        "end": 4621606,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_switch_vector4.osl",
                        "start": 4621606,
                        "end": 4622580,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color4_color.osl",
                        "start": 4622580,
                        "end": 4623257,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color4_color4.osl",
                        "start": 4623257,
                        "end": 4623953,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color4_float.osl",
                        "start": 4623953,
                        "end": 4624630,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color4_vector.osl",
                        "start": 4624630,
                        "end": 4625310,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color4_vector2.osl",
                        "start": 4625310,
                        "end": 4625997,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color4_vector4.osl",
                        "start": 4625997,
                        "end": 4626688,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color_color.osl",
                        "start": 4626688,
                        "end": 4627318,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color_color4.osl",
                        "start": 4627318,
                        "end": 4627967,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color_float.osl",
                        "start": 4627967,
                        "end": 4628597,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color_vector.osl",
                        "start": 4628597,
                        "end": 4629230,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color_vector2.osl",
                        "start": 4629230,
                        "end": 4629870,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_color_vector4.osl",
                        "start": 4629870,
                        "end": 4630514,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_float_color.osl",
                        "start": 4630514,
                        "end": 4631152,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_float_color4.osl",
                        "start": 4631152,
                        "end": 4631809,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_float_vector.osl",
                        "start": 4631809,
                        "end": 4632450,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_float_vector2.osl",
                        "start": 4632450,
                        "end": 4633098,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_float_vector4.osl",
                        "start": 4633098,
                        "end": 4633750,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector2_color.osl",
                        "start": 4633750,
                        "end": 4634365,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector2_color4.osl",
                        "start": 4634365,
                        "end": 4634999,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector2_float.osl",
                        "start": 4634999,
                        "end": 4635614,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector2_vector.osl",
                        "start": 4635614,
                        "end": 4636232,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector2_vector2.osl",
                        "start": 4636232,
                        "end": 4636857,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector2_vector4.osl",
                        "start": 4636857,
                        "end": 4637486,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector4_color.osl",
                        "start": 4637486,
                        "end": 4638150,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector4_color4.osl",
                        "start": 4638150,
                        "end": 4638833,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector4_float.osl",
                        "start": 4638833,
                        "end": 4639497,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector4_vector.osl",
                        "start": 4639497,
                        "end": 4640164,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector4_vector2.osl",
                        "start": 4640164,
                        "end": 4640838,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector4_vector4.osl",
                        "start": 4640838,
                        "end": 4641516,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector_color.osl",
                        "start": 4641516,
                        "end": 4642148,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector_color4.osl",
                        "start": 4642148,
                        "end": 4642799,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector_float.osl",
                        "start": 4642799,
                        "end": 4643431,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector_vector.osl",
                        "start": 4643431,
                        "end": 4644066,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector_vector2.osl",
                        "start": 4644066,
                        "end": 4644708,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_swizzle_vector_vector4.osl",
                        "start": 4644708,
                        "end": 4645354,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tangent_vector.osl",
                        "start": 4645354,
                        "end": 4645821,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tan_color.osl",
                        "start": 4645821,
                        "end": 4646243,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tan_color4.osl",
                        "start": 4646243,
                        "end": 4646701,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tan_float.osl",
                        "start": 4646701,
                        "end": 4647123,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tan_vector.osl",
                        "start": 4647123,
                        "end": 4647549,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tan_vector2.osl",
                        "start": 4647549,
                        "end": 4647987,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tan_vector4.osl",
                        "start": 4647987,
                        "end": 4648433,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_texcoord_vector.osl",
                        "start": 4648433,
                        "end": 4649013,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_texcoord_vector2.osl",
                        "start": 4649013,
                        "end": 4649599,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tiledimage_color.osl",
                        "start": 4649599,
                        "end": 4651245,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tiledimage_color4.osl",
                        "start": 4651245,
                        "end": 4652926,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tiledimage_float.osl",
                        "start": 4652926,
                        "end": 4654572,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tiledimage_vector.osl",
                        "start": 4654572,
                        "end": 4656221,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tiledimage_vector2.osl",
                        "start": 4656221,
                        "end": 4657881,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_tiledimage_vector4.osl",
                        "start": 4657881,
                        "end": 4659549,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_time_float.osl",
                        "start": 4659549,
                        "end": 4660013,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transformnormal_vector.osl",
                        "start": 4660013,
                        "end": 4660923,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transformnormal_vector4.osl",
                        "start": 4660923,
                        "end": 4661851,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transformpoint_vector.osl",
                        "start": 4661851,
                        "end": 4662759,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transformpoint_vector4.osl",
                        "start": 4662759,
                        "end": 4663685,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transformvector_vector.osl",
                        "start": 4663685,
                        "end": 4664595,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transformvector_vector4.osl",
                        "start": 4664595,
                        "end": 4665523,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transpose_matrix33.osl",
                        "start": 4665523,
                        "end": 4666e3,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_transpose_matrix44.osl",
                        "start": 4666e3,
                        "end": 4666477,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_triplanarprojection_color.osl",
                        "start": 4666477,
                        "end": 4669245,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_triplanarprojection_color4.osl",
                        "start": 4669245,
                        "end": 4672048,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_triplanarprojection_float.osl",
                        "start": 4672048,
                        "end": 4674816,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_triplanarprojection_vector.osl",
                        "start": 4674816,
                        "end": 4677587,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_triplanarprojection_vector2.osl",
                        "start": 4677587,
                        "end": 4680369,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_triplanarprojection_vector4.osl",
                        "start": 4680369,
                        "end": 4683159,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_unpremult_color.osl",
                        "start": 4683159,
                        "end": 4683998,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_unpremult_color4.osl",
                        "start": 4683998,
                        "end": 4684873,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/mx_viewdirection_vector.osl",
                        "start": 4684873,
                        "end": 4685271,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/oslutil.h",
                        "start": 4685271,
                        "end": 4692007,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/README.md",
                        "start": 4692007,
                        "end": 4692541,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/stdlib_osl_impl.mtlx",
                        "start": 4692541,
                        "end": 4784928,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/stdosl.h",
                        "start": 4784928,
                        "end": 4823102,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/vector2.h",
                        "start": 4823102,
                        "end": 4828935,
                        "audio": 0
                    }, {
                        "filename": "/libraries/stdlib/osl/vector4.h",
                        "start": 4828935,
                        "end": 4837887,
                        "audio": 0
                    }, {
                        "filename": "/libraries/targets/arnold.mtlx",
                        "start": 4837887,
                        "end": 4838415,
                        "audio": 0
                    }, {
                        "filename": "/libraries/targets/essl.mtlx",
                        "start": 4838415,
                        "end": 4838943,
                        "audio": 0
                    }, {
                        "filename": "/libraries/targets/genglsl.mtlx",
                        "start": 4838943,
                        "end": 4839456,
                        "audio": 0
                    }, {
                        "filename": "/libraries/targets/genmdl.mtlx",
                        "start": 4839456,
                        "end": 4839968,
                        "audio": 0
                    }, {
                        "filename": "/libraries/targets/genosl.mtlx",
                        "start": 4839968,
                        "end": 4840480,
                        "audio": 0
                    }, {
                        "filename": "/libraries/targets/ogsfx.mtlx",
                        "start": 4840480,
                        "end": 4841008,
                        "audio": 0
                    }, {"filename": "/libraries/targets/ogsxml.mtlx", "start": 4841008, "end": 4841537, "audio": 0}],
                    "remote_package_size": 4841537,
                    "package_uuid": "653a20bf-4589-46d0-9401-0fb288dc202a"
                })
            })();
            var moduleOverrides = {};
            var key;
            for (key in Module) {
                if (Module.hasOwnProperty(key)) {
                    moduleOverrides[key] = Module[key]
                }
            }
            var arguments_ = [];
            var thisProgram = "./this.program";
            var quit_ = function (status, toThrow) {
                throw toThrow
            };
            var ENVIRONMENT_IS_WEB = typeof window === "object";
            var ENVIRONMENT_IS_WORKER = typeof importScripts === "function";
            var ENVIRONMENT_IS_NODE = typeof process === "object" && typeof process.versions === "object" && typeof process.versions.node === "string";
            var scriptDirectory = "";

            function locateFile(path) {
                if (Module["locateFile"]) {
                    return Module["locateFile"](path, scriptDirectory)
                }
                return scriptDirectory + path
            }

            var read_, readAsync, readBinary, setWindowTitle;
            var nodeFS;
            var nodePath;
            if (ENVIRONMENT_IS_NODE) {
                if (ENVIRONMENT_IS_WORKER) {
                    scriptDirectory = require("path").dirname(scriptDirectory) + "/"
                } else {
                    scriptDirectory = __dirname + "/"
                }
                read_ = function shell_read(filename, binary) {
                    if (!nodeFS) nodeFS = require("fs");
                    if (!nodePath) nodePath = require("path");
                    filename = nodePath["normalize"](filename);
                    return nodeFS["readFileSync"](filename, binary ? null : "utf8")
                };
                readBinary = function readBinary(filename) {
                    var ret = read_(filename, true);
                    if (!ret.buffer) {
                        ret = new Uint8Array(ret)
                    }
                    assert(ret.buffer);
                    return ret
                };
                readAsync = function readAsync(filename, onload, onerror) {
                    if (!nodeFS) nodeFS = require("fs");
                    if (!nodePath) nodePath = require("path");
                    filename = nodePath["normalize"](filename);
                    nodeFS["readFile"](filename, function (err, data) {
                        if (err) onerror(err); else onload(data.buffer)
                    })
                };
                if (process["argv"].length > 1) {
                    thisProgram = process["argv"][1].replace(/\\/g, "/")
                }
                arguments_ = process["argv"].slice(2);
                process["on"]("uncaughtException", function (ex) {
                    if (!(ex instanceof ExitStatus)) {
                        throw ex
                    }
                });
                process["on"]("unhandledRejection", abort);
                quit_ = function (status, toThrow) {
                    if (keepRuntimeAlive()) {
                        process["exitCode"] = status;
                        throw toThrow
                    }
                    process["exit"](status)
                };
                Module["inspect"] = function () {
                    return "[Emscripten Module object]"
                }
            } else if (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER) {
                if (ENVIRONMENT_IS_WORKER) {
                    scriptDirectory = self.location.href
                } else if (typeof document !== "undefined" && document.currentScript) {
                    scriptDirectory = document.currentScript.src
                }
                if (_scriptDir) {
                    scriptDirectory = _scriptDir
                }
                if (scriptDirectory.indexOf("blob:") !== 0) {
                    scriptDirectory = scriptDirectory.substr(0, scriptDirectory.lastIndexOf("/") + 1)
                } else {
                    scriptDirectory = ""
                }
                {
                    read_ = function (url) {
                        var xhr = new XMLHttpRequest;
                        xhr.open("GET", url, false);
                        xhr.send(null);
                        return xhr.responseText
                    };
                    if (ENVIRONMENT_IS_WORKER) {
                        readBinary = function (url) {
                            var xhr = new XMLHttpRequest;
                            xhr.open("GET", url, false);
                            xhr.responseType = "arraybuffer";
                            xhr.send(null);
                            return new Uint8Array(xhr.response)
                        }
                    }
                    readAsync = function (url, onload, onerror) {
                        var xhr = new XMLHttpRequest;
                        xhr.open("GET", url, true);
                        xhr.responseType = "arraybuffer";
                        xhr.onload = function () {
                            if (xhr.status == 200 || xhr.status == 0 && xhr.response) {
                                onload(xhr.response);
                                return
                            }
                            onerror()
                        };
                        xhr.onerror = onerror;
                        xhr.send(null)
                    }
                }
                setWindowTitle = function (title) {
                    document.title = title
                }
            } else {
            }
            var out = Module["print"] || console.log.bind(console);
            var err = Module["printErr"] || console.warn.bind(console);
            for (key in moduleOverrides) {
                if (moduleOverrides.hasOwnProperty(key)) {
                    Module[key] = moduleOverrides[key]
                }
            }
            moduleOverrides = null;
            if (Module["arguments"]) arguments_ = Module["arguments"];
            if (Module["thisProgram"]) thisProgram = Module["thisProgram"];
            if (Module["quit"]) quit_ = Module["quit"];
            var tempRet0 = 0;
            var setTempRet0 = function (value) {
                tempRet0 = value
            };
            var getTempRet0 = function () {
                return tempRet0
            };
            var wasmBinary;
            if (Module["wasmBinary"]) wasmBinary = Module["wasmBinary"];
            var noExitRuntime = Module["noExitRuntime"] || true;
            if (typeof WebAssembly !== "object") {
                abort("no native wasm support detected")
            }
            var wasmMemory;
            var ABORT = false;
            var EXITSTATUS;

            function assert(condition, text) {
                if (!condition) {
                    abort("Assertion failed: " + text)
                }
            }

            var UTF8Decoder = typeof TextDecoder !== "undefined" ? new TextDecoder("utf8") : undefined;

            function UTF8ArrayToString(heap, idx, maxBytesToRead) {
                var endIdx = idx + maxBytesToRead;
                var endPtr = idx;
                while (heap[endPtr] && !(endPtr >= endIdx)) ++endPtr;
                if (endPtr - idx > 16 && heap.subarray && UTF8Decoder) {
                    return UTF8Decoder.decode(heap.subarray(idx, endPtr))
                } else {
                    var str = "";
                    while (idx < endPtr) {
                        var u0 = heap[idx++];
                        if (!(u0 & 128)) {
                            str += String.fromCharCode(u0);
                            continue
                        }
                        var u1 = heap[idx++] & 63;
                        if ((u0 & 224) == 192) {
                            str += String.fromCharCode((u0 & 31) << 6 | u1);
                            continue
                        }
                        var u2 = heap[idx++] & 63;
                        if ((u0 & 240) == 224) {
                            u0 = (u0 & 15) << 12 | u1 << 6 | u2
                        } else {
                            u0 = (u0 & 7) << 18 | u1 << 12 | u2 << 6 | heap[idx++] & 63
                        }
                        if (u0 < 65536) {
                            str += String.fromCharCode(u0)
                        } else {
                            var ch = u0 - 65536;
                            str += String.fromCharCode(55296 | ch >> 10, 56320 | ch & 1023)
                        }
                    }
                }
                return str
            }

            function UTF8ToString(ptr, maxBytesToRead) {
                return ptr ? UTF8ArrayToString(HEAPU8, ptr, maxBytesToRead) : ""
            }

            function stringToUTF8Array(str, heap, outIdx, maxBytesToWrite) {
                if (!(maxBytesToWrite > 0)) return 0;
                var startIdx = outIdx;
                var endIdx = outIdx + maxBytesToWrite - 1;
                for (var i = 0; i < str.length; ++i) {
                    var u = str.charCodeAt(i);
                    if (u >= 55296 && u <= 57343) {
                        var u1 = str.charCodeAt(++i);
                        u = 65536 + ((u & 1023) << 10) | u1 & 1023
                    }
                    if (u <= 127) {
                        if (outIdx >= endIdx) break;
                        heap[outIdx++] = u
                    } else if (u <= 2047) {
                        if (outIdx + 1 >= endIdx) break;
                        heap[outIdx++] = 192 | u >> 6;
                        heap[outIdx++] = 128 | u & 63
                    } else if (u <= 65535) {
                        if (outIdx + 2 >= endIdx) break;
                        heap[outIdx++] = 224 | u >> 12;
                        heap[outIdx++] = 128 | u >> 6 & 63;
                        heap[outIdx++] = 128 | u & 63
                    } else {
                        if (outIdx + 3 >= endIdx) break;
                        heap[outIdx++] = 240 | u >> 18;
                        heap[outIdx++] = 128 | u >> 12 & 63;
                        heap[outIdx++] = 128 | u >> 6 & 63;
                        heap[outIdx++] = 128 | u & 63
                    }
                }
                heap[outIdx] = 0;
                return outIdx - startIdx
            }

            function stringToUTF8(str, outPtr, maxBytesToWrite) {
                return stringToUTF8Array(str, HEAPU8, outPtr, maxBytesToWrite)
            }

            function lengthBytesUTF8(str) {
                var len = 0;
                for (var i = 0; i < str.length; ++i) {
                    var u = str.charCodeAt(i);
                    if (u >= 55296 && u <= 57343) u = 65536 + ((u & 1023) << 10) | str.charCodeAt(++i) & 1023;
                    if (u <= 127) ++len; else if (u <= 2047) len += 2; else if (u <= 65535) len += 3; else len += 4
                }
                return len
            }

            var UTF16Decoder = typeof TextDecoder !== "undefined" ? new TextDecoder("utf-16le") : undefined;

            function UTF16ToString(ptr, maxBytesToRead) {
                var endPtr = ptr;
                var idx = endPtr >> 1;
                var maxIdx = idx + maxBytesToRead / 2;
                while (!(idx >= maxIdx) && HEAPU16[idx]) ++idx;
                endPtr = idx << 1;
                if (endPtr - ptr > 32 && UTF16Decoder) {
                    return UTF16Decoder.decode(HEAPU8.subarray(ptr, endPtr))
                } else {
                    var str = "";
                    for (var i = 0; !(i >= maxBytesToRead / 2); ++i) {
                        var codeUnit = HEAP16[ptr + i * 2 >> 1];
                        if (codeUnit == 0) break;
                        str += String.fromCharCode(codeUnit)
                    }
                    return str
                }
            }

            function stringToUTF16(str, outPtr, maxBytesToWrite) {
                if (maxBytesToWrite === undefined) {
                    maxBytesToWrite = 2147483647
                }
                if (maxBytesToWrite < 2) return 0;
                maxBytesToWrite -= 2;
                var startPtr = outPtr;
                var numCharsToWrite = maxBytesToWrite < str.length * 2 ? maxBytesToWrite / 2 : str.length;
                for (var i = 0; i < numCharsToWrite; ++i) {
                    var codeUnit = str.charCodeAt(i);
                    HEAP16[outPtr >> 1] = codeUnit;
                    outPtr += 2
                }
                HEAP16[outPtr >> 1] = 0;
                return outPtr - startPtr
            }

            function lengthBytesUTF16(str) {
                return str.length * 2
            }

            function UTF32ToString(ptr, maxBytesToRead) {
                var i = 0;
                var str = "";
                while (!(i >= maxBytesToRead / 4)) {
                    var utf32 = HEAP32[ptr + i * 4 >> 2];
                    if (utf32 == 0) break;
                    ++i;
                    if (utf32 >= 65536) {
                        var ch = utf32 - 65536;
                        str += String.fromCharCode(55296 | ch >> 10, 56320 | ch & 1023)
                    } else {
                        str += String.fromCharCode(utf32)
                    }
                }
                return str
            }

            function stringToUTF32(str, outPtr, maxBytesToWrite) {
                if (maxBytesToWrite === undefined) {
                    maxBytesToWrite = 2147483647
                }
                if (maxBytesToWrite < 4) return 0;
                var startPtr = outPtr;
                var endPtr = startPtr + maxBytesToWrite - 4;
                for (var i = 0; i < str.length; ++i) {
                    var codeUnit = str.charCodeAt(i);
                    if (codeUnit >= 55296 && codeUnit <= 57343) {
                        var trailSurrogate = str.charCodeAt(++i);
                        codeUnit = 65536 + ((codeUnit & 1023) << 10) | trailSurrogate & 1023
                    }
                    HEAP32[outPtr >> 2] = codeUnit;
                    outPtr += 4;
                    if (outPtr + 4 > endPtr) break
                }
                HEAP32[outPtr >> 2] = 0;
                return outPtr - startPtr
            }

            function lengthBytesUTF32(str) {
                var len = 0;
                for (var i = 0; i < str.length; ++i) {
                    var codeUnit = str.charCodeAt(i);
                    if (codeUnit >= 55296 && codeUnit <= 57343) ++i;
                    len += 4
                }
                return len
            }

            function writeArrayToMemory(array, buffer) {
                HEAP8.set(array, buffer)
            }

            function writeAsciiToMemory(str, buffer, dontAddNull) {
                for (var i = 0; i < str.length; ++i) {
                    HEAP8[buffer++ >> 0] = str.charCodeAt(i)
                }
                if (!dontAddNull) HEAP8[buffer >> 0] = 0
            }

            function alignUp(x, multiple) {
                if (x % multiple > 0) {
                    x += multiple - x % multiple
                }
                return x
            }

            var buffer, HEAP8, HEAPU8, HEAP16, HEAPU16, HEAP32, HEAPU32, HEAPF32, HEAPF64;

            function updateGlobalBufferAndViews(buf) {
                buffer = buf;
                Module["HEAP8"] = HEAP8 = new Int8Array(buf);
                Module["HEAP16"] = HEAP16 = new Int16Array(buf);
                Module["HEAP32"] = HEAP32 = new Int32Array(buf);
                Module["HEAPU8"] = HEAPU8 = new Uint8Array(buf);
                Module["HEAPU16"] = HEAPU16 = new Uint16Array(buf);
                Module["HEAPU32"] = HEAPU32 = new Uint32Array(buf);
                Module["HEAPF32"] = HEAPF32 = new Float32Array(buf);
                Module["HEAPF64"] = HEAPF64 = new Float64Array(buf)
            }

            var INITIAL_MEMORY = Module["INITIAL_MEMORY"] || 16777216;
            var wasmTable;
            var __ATPRERUN__ = [];
            var __ATINIT__ = [];
            var __ATPOSTRUN__ = [];
            var runtimeInitialized = false;
            var runtimeKeepaliveCounter = 0;

            function keepRuntimeAlive() {
                return noExitRuntime || runtimeKeepaliveCounter > 0
            }

            function preRun() {
                if (Module["preRun"]) {
                    if (typeof Module["preRun"] == "function") Module["preRun"] = [Module["preRun"]];
                    while (Module["preRun"].length) {
                        addOnPreRun(Module["preRun"].shift())
                    }
                }
                callRuntimeCallbacks(__ATPRERUN__)
            }

            function initRuntime() {
                runtimeInitialized = true;
                if (!Module["noFSInit"] && !FS.init.initialized) FS.init();
                FS.ignorePermissions = false;
                TTY.init();
                callRuntimeCallbacks(__ATINIT__)
            }

            function postRun() {
                if (Module["postRun"]) {
                    if (typeof Module["postRun"] == "function") Module["postRun"] = [Module["postRun"]];
                    while (Module["postRun"].length) {
                        addOnPostRun(Module["postRun"].shift())
                    }
                }
                callRuntimeCallbacks(__ATPOSTRUN__)
            }

            function addOnPreRun(cb) {
                __ATPRERUN__.unshift(cb)
            }

            function addOnInit(cb) {
                __ATINIT__.unshift(cb)
            }

            function addOnPostRun(cb) {
                __ATPOSTRUN__.unshift(cb)
            }

            var runDependencies = 0;
            var runDependencyWatcher = null;
            var dependenciesFulfilled = null;

            function getUniqueRunDependency(id) {
                return id
            }

            function addRunDependency(id) {
                runDependencies++;
                if (Module["monitorRunDependencies"]) {
                    Module["monitorRunDependencies"](runDependencies)
                }
            }

            function removeRunDependency(id) {
                runDependencies--;
                if (Module["monitorRunDependencies"]) {
                    Module["monitorRunDependencies"](runDependencies)
                }
                if (runDependencies == 0) {
                    if (runDependencyWatcher !== null) {
                        clearInterval(runDependencyWatcher);
                        runDependencyWatcher = null
                    }
                    if (dependenciesFulfilled) {
                        var callback = dependenciesFulfilled;
                        dependenciesFulfilled = null;
                        callback()
                    }
                }
            }

            Module["preloadedImages"] = {};
            Module["preloadedAudios"] = {};

            function abort(what) {
                if (Module["onAbort"]) {
                    Module["onAbort"](what)
                }
                what += "";
                err(what);
                ABORT = true;
                EXITSTATUS = 1;
                what = "abort(" + what + "). Build with -s ASSERTIONS=1 for more info.";
                var e = new WebAssembly.RuntimeError(what);
                readyPromiseReject(e);
                throw e
            }

            var dataURIPrefix = "data:application/octet-stream;base64,";

            function isDataURI(filename) {
                return filename.startsWith(dataURIPrefix)
            }

            function isFileURI(filename) {
                return filename.startsWith("file://")
            }

            var wasmBinaryFile;
            wasmBinaryFile = "JsMaterialXGenShader.wasm";
            if (!isDataURI(wasmBinaryFile)) {
                wasmBinaryFile = locateFile(wasmBinaryFile)
            }

            function getBinary(file) {
                try {
                    if (file == wasmBinaryFile && wasmBinary) {
                        return new Uint8Array(wasmBinary)
                    }
                    if (readBinary) {
                        return readBinary(file)
                    } else {
                        throw"both async and sync fetching of the wasm failed"
                    }
                } catch (err) {
                    abort(err)
                }
            }

            function getBinaryPromise() {
                if (!wasmBinary && (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER)) {
                    if (typeof fetch === "function" && !isFileURI(wasmBinaryFile)) {
                        return fetch(wasmBinaryFile, {credentials: "same-origin"}).then(function (response) {
                            if (!response["ok"]) {
                                throw"failed to load wasm binary file at '" + wasmBinaryFile + "'"
                            }
                            return response["arrayBuffer"]()
                        }).catch(function () {
                            return getBinary(wasmBinaryFile)
                        })
                    } else {
                        if (readAsync) {
                            return new Promise(function (resolve, reject) {
                                readAsync(wasmBinaryFile, function (response) {
                                    resolve(new Uint8Array(response))
                                }, reject)
                            })
                        }
                    }
                }
                return Promise.resolve().then(function () {
                    return getBinary(wasmBinaryFile)
                })
            }

            function createWasm() {
                var info = {"a": asmLibraryArg};

                function receiveInstance(instance, module) {
                    var exports = instance.exports;
                    Module["asm"] = exports;
                    wasmMemory = Module["asm"]["Xa"];
                    updateGlobalBufferAndViews(wasmMemory.buffer);
                    wasmTable = Module["asm"]["_a"];
                    addOnInit(Module["asm"]["Ya"]);
                    removeRunDependency("wasm-instantiate")
                }

                addRunDependency("wasm-instantiate");

                function receiveInstantiationResult(result) {
                    receiveInstance(result["instance"])
                }

                function instantiateArrayBuffer(receiver) {
                    return getBinaryPromise().then(function (binary) {
                        var result = WebAssembly.instantiate(binary, info);
                        return result
                    }).then(receiver, function (reason) {
                        err("failed to asynchronously prepare wasm: " + reason);
                        abort(reason)
                    })
                }

                function instantiateAsync() {
                    if (!wasmBinary && typeof WebAssembly.instantiateStreaming === "function" && !isDataURI(wasmBinaryFile) && !isFileURI(wasmBinaryFile) && typeof fetch === "function") {
                        return fetch(wasmBinaryFile, {credentials: "same-origin"}).then(function (response) {
                            var result = WebAssembly.instantiateStreaming(response, info);
                            return result.then(receiveInstantiationResult, function (reason) {
                                err("wasm streaming compile failed: " + reason);
                                err("falling back to ArrayBuffer instantiation");
                                return instantiateArrayBuffer(receiveInstantiationResult)
                            })
                        })
                    } else {
                        return instantiateArrayBuffer(receiveInstantiationResult)
                    }
                }

                if (Module["instantiateWasm"]) {
                    try {
                        var exports = Module["instantiateWasm"](info, receiveInstance);
                        return exports
                    } catch (e) {
                        err("Module.instantiateWasm callback failed with error: " + e);
                        return false
                    }
                }
                instantiateAsync().catch(readyPromiseReject);
                return {}
            }

            var tempDouble;
            var tempI64;
            var ASM_CONSTS = {
                159932: function () {
                    Module["TreeIterator"]["prototype"][Symbol.iterator] = function () {
                        return this
                    }
                }, 160016: function () {
                    Module["GraphIterator"]["prototype"][Symbol.iterator] = function () {
                        return this
                    }
                }, 160101: function () {
                    Module["InheritanceIterator"]["prototype"][Symbol.iterator] = function () {
                        return this
                    }
                }
            };

            function callRuntimeCallbacks(callbacks) {
                while (callbacks.length > 0) {
                    var callback = callbacks.shift();
                    if (typeof callback == "function") {
                        callback(Module);
                        continue
                    }
                    var func = callback.func;
                    if (typeof func === "number") {
                        if (callback.arg === undefined) {
                            wasmTable.get(func)()
                        } else {
                            wasmTable.get(func)(callback.arg)
                        }
                    } else {
                        func(callback.arg === undefined ? null : callback.arg)
                    }
                }
            }

            function ___assert_fail(condition, filename, line, func) {
                abort("Assertion failed: " + UTF8ToString(condition) + ", at: " + [filename ? UTF8ToString(filename) : "unknown filename", line, func ? UTF8ToString(func) : "unknown function"])
            }

            function ___cxa_allocate_exception(size) {
                return _malloc(size + 16) + 16
            }

            function ExceptionInfo(excPtr) {
                this.excPtr = excPtr;
                this.ptr = excPtr - 16;
                this.set_type = function (type) {
                    HEAP32[this.ptr + 4 >> 2] = type
                };
                this.get_type = function () {
                    return HEAP32[this.ptr + 4 >> 2]
                };
                this.set_destructor = function (destructor) {
                    HEAP32[this.ptr + 8 >> 2] = destructor
                };
                this.get_destructor = function () {
                    return HEAP32[this.ptr + 8 >> 2]
                };
                this.set_refcount = function (refcount) {
                    HEAP32[this.ptr >> 2] = refcount
                };
                this.set_caught = function (caught) {
                    caught = caught ? 1 : 0;
                    HEAP8[this.ptr + 12 >> 0] = caught
                };
                this.get_caught = function () {
                    return HEAP8[this.ptr + 12 >> 0] != 0
                };
                this.set_rethrown = function (rethrown) {
                    rethrown = rethrown ? 1 : 0;
                    HEAP8[this.ptr + 13 >> 0] = rethrown
                };
                this.get_rethrown = function () {
                    return HEAP8[this.ptr + 13 >> 0] != 0
                };
                this.init = function (type, destructor) {
                    this.set_type(type);
                    this.set_destructor(destructor);
                    this.set_refcount(0);
                    this.set_caught(false);
                    this.set_rethrown(false)
                };
                this.add_ref = function () {
                    var value = HEAP32[this.ptr >> 2];
                    HEAP32[this.ptr >> 2] = value + 1
                };
                this.release_ref = function () {
                    var prev = HEAP32[this.ptr >> 2];
                    HEAP32[this.ptr >> 2] = prev - 1;
                    return prev === 1
                }
            }

            function CatchInfo(ptr) {
                this.free = function () {
                    _free(this.ptr);
                    this.ptr = 0
                };
                this.set_base_ptr = function (basePtr) {
                    HEAP32[this.ptr >> 2] = basePtr
                };
                this.get_base_ptr = function () {
                    return HEAP32[this.ptr >> 2]
                };
                this.set_adjusted_ptr = function (adjustedPtr) {
                    HEAP32[this.ptr + 4 >> 2] = adjustedPtr
                };
                this.get_adjusted_ptr_addr = function () {
                    return this.ptr + 4
                };
                this.get_adjusted_ptr = function () {
                    return HEAP32[this.ptr + 4 >> 2]
                };
                this.get_exception_ptr = function () {
                    var isPointer = ___cxa_is_pointer_type(this.get_exception_info().get_type());
                    if (isPointer) {
                        return HEAP32[this.get_base_ptr() >> 2]
                    }
                    var adjusted = this.get_adjusted_ptr();
                    if (adjusted !== 0) return adjusted;
                    return this.get_base_ptr()
                };
                this.get_exception_info = function () {
                    return new ExceptionInfo(this.get_base_ptr())
                };
                if (ptr === undefined) {
                    this.ptr = _malloc(8);
                    this.set_adjusted_ptr(0)
                } else {
                    this.ptr = ptr
                }
            }

            var exceptionCaught = [];

            function exception_addRef(info) {
                info.add_ref()
            }

            var uncaughtExceptionCount = 0;

            function ___cxa_begin_catch(ptr) {
                var catchInfo = new CatchInfo(ptr);
                var info = catchInfo.get_exception_info();
                if (!info.get_caught()) {
                    info.set_caught(true);
                    uncaughtExceptionCount--
                }
                info.set_rethrown(false);
                exceptionCaught.push(catchInfo);
                exception_addRef(info);
                return catchInfo.get_exception_ptr()
            }

            var exceptionLast = 0;

            function ___cxa_free_exception(ptr) {
                return _free(new ExceptionInfo(ptr).ptr)
            }

            function exception_decRef(info) {
                if (info.release_ref() && !info.get_rethrown()) {
                    var destructor = info.get_destructor();
                    if (destructor) {
                        wasmTable.get(destructor)(info.excPtr)
                    }
                    ___cxa_free_exception(info.excPtr)
                }
            }

            function ___cxa_end_catch() {
                _setThrew(0);
                var catchInfo = exceptionCaught.pop();
                exception_decRef(catchInfo.get_exception_info());
                catchInfo.free();
                exceptionLast = 0
            }

            function ___resumeException(catchInfoPtr) {
                var catchInfo = new CatchInfo(catchInfoPtr);
                var ptr = catchInfo.get_base_ptr();
                if (!exceptionLast) {
                    exceptionLast = ptr
                }
                catchInfo.free();
                throw ptr
            }

            function ___cxa_find_matching_catch_2() {
                var thrown = exceptionLast;
                if (!thrown) {
                    setTempRet0(0);
                    return 0 | 0
                }
                var info = new ExceptionInfo(thrown);
                var thrownType = info.get_type();
                var catchInfo = new CatchInfo;
                catchInfo.set_base_ptr(thrown);
                catchInfo.set_adjusted_ptr(thrown);
                if (!thrownType) {
                    setTempRet0(0);
                    return catchInfo.ptr | 0
                }
                var typeArray = Array.prototype.slice.call(arguments);
                for (var i = 0; i < typeArray.length; i++) {
                    var caughtType = typeArray[i];
                    if (caughtType === 0 || caughtType === thrownType) {
                        break
                    }
                    if (___cxa_can_catch(caughtType, thrownType, catchInfo.get_adjusted_ptr_addr())) {
                        setTempRet0(caughtType);
                        return catchInfo.ptr | 0
                    }
                }
                setTempRet0(thrownType);
                return catchInfo.ptr | 0
            }

            function ___cxa_find_matching_catch_3() {
                var thrown = exceptionLast;
                if (!thrown) {
                    setTempRet0(0);
                    return 0 | 0
                }
                var info = new ExceptionInfo(thrown);
                var thrownType = info.get_type();
                var catchInfo = new CatchInfo;
                catchInfo.set_base_ptr(thrown);
                catchInfo.set_adjusted_ptr(thrown);
                if (!thrownType) {
                    setTempRet0(0);
                    return catchInfo.ptr | 0
                }
                var typeArray = Array.prototype.slice.call(arguments);
                for (var i = 0; i < typeArray.length; i++) {
                    var caughtType = typeArray[i];
                    if (caughtType === 0 || caughtType === thrownType) {
                        break
                    }
                    if (___cxa_can_catch(caughtType, thrownType, catchInfo.get_adjusted_ptr_addr())) {
                        setTempRet0(caughtType);
                        return catchInfo.ptr | 0
                    }
                }
                setTempRet0(thrownType);
                return catchInfo.ptr | 0
            }

            function ___cxa_find_matching_catch_4() {
                var thrown = exceptionLast;
                if (!thrown) {
                    setTempRet0(0);
                    return 0 | 0
                }
                var info = new ExceptionInfo(thrown);
                var thrownType = info.get_type();
                var catchInfo = new CatchInfo;
                catchInfo.set_base_ptr(thrown);
                catchInfo.set_adjusted_ptr(thrown);
                if (!thrownType) {
                    setTempRet0(0);
                    return catchInfo.ptr | 0
                }
                var typeArray = Array.prototype.slice.call(arguments);
                for (var i = 0; i < typeArray.length; i++) {
                    var caughtType = typeArray[i];
                    if (caughtType === 0 || caughtType === thrownType) {
                        break
                    }
                    if (___cxa_can_catch(caughtType, thrownType, catchInfo.get_adjusted_ptr_addr())) {
                        setTempRet0(caughtType);
                        return catchInfo.ptr | 0
                    }
                }
                setTempRet0(thrownType);
                return catchInfo.ptr | 0
            }

            function ___cxa_rethrow() {
                var catchInfo = exceptionCaught.pop();
                if (!catchInfo) {
                    abort("no exception to throw")
                }
                var info = catchInfo.get_exception_info();
                var ptr = catchInfo.get_base_ptr();
                if (!info.get_rethrown()) {
                    exceptionCaught.push(catchInfo);
                    info.set_rethrown(true);
                    info.set_caught(false);
                    uncaughtExceptionCount++
                } else {
                    catchInfo.free()
                }
                exceptionLast = ptr;
                throw ptr
            }

            function ___cxa_throw(ptr, type, destructor) {
                var info = new ExceptionInfo(ptr);
                info.init(type, destructor);
                exceptionLast = ptr;
                uncaughtExceptionCount++;
                throw ptr
            }

            function ___cxa_uncaught_exceptions() {
                return uncaughtExceptionCount
            }

            function setErrNo(value) {
                HEAP32[___errno_location() >> 2] = value;
                return value
            }

            var PATH = {
                splitPath: function (filename) {
                    var splitPathRe = /^(\/?|)([\s\S]*?)((?:\.{1,2}|[^\/]+?|)(\.[^.\/]*|))(?:[\/]*)$/;
                    return splitPathRe.exec(filename).slice(1)
                }, normalizeArray: function (parts, allowAboveRoot) {
                    var up = 0;
                    for (var i = parts.length - 1; i >= 0; i--) {
                        var last = parts[i];
                        if (last === ".") {
                            parts.splice(i, 1)
                        } else if (last === "..") {
                            parts.splice(i, 1);
                            up++
                        } else if (up) {
                            parts.splice(i, 1);
                            up--
                        }
                    }
                    if (allowAboveRoot) {
                        for (; up; up--) {
                            parts.unshift("..")
                        }
                    }
                    return parts
                }, normalize: function (path) {
                    var isAbsolute = path.charAt(0) === "/", trailingSlash = path.substr(-1) === "/";
                    path = PATH.normalizeArray(path.split("/").filter(function (p) {
                        return !!p
                    }), !isAbsolute).join("/");
                    if (!path && !isAbsolute) {
                        path = "."
                    }
                    if (path && trailingSlash) {
                        path += "/"
                    }
                    return (isAbsolute ? "/" : "") + path
                }, dirname: function (path) {
                    var result = PATH.splitPath(path), root = result[0], dir = result[1];
                    if (!root && !dir) {
                        return "."
                    }
                    if (dir) {
                        dir = dir.substr(0, dir.length - 1)
                    }
                    return root + dir
                }, basename: function (path) {
                    if (path === "/") return "/";
                    path = PATH.normalize(path);
                    path = path.replace(/\/$/, "");
                    var lastSlash = path.lastIndexOf("/");
                    if (lastSlash === -1) return path;
                    return path.substr(lastSlash + 1)
                }, extname: function (path) {
                    return PATH.splitPath(path)[3]
                }, join: function () {
                    var paths = Array.prototype.slice.call(arguments, 0);
                    return PATH.normalize(paths.join("/"))
                }, join2: function (l, r) {
                    return PATH.normalize(l + "/" + r)
                }
            };

            function getRandomDevice() {
                if (typeof crypto === "object" && typeof crypto["getRandomValues"] === "function") {
                    var randomBuffer = new Uint8Array(1);
                    return function () {
                        crypto.getRandomValues(randomBuffer);
                        return randomBuffer[0]
                    }
                } else if (ENVIRONMENT_IS_NODE) {
                    try {
                        var crypto_module = require("crypto");
                        return function () {
                            return crypto_module["randomBytes"](1)[0]
                        }
                    } catch (e) {
                    }
                }
                return function () {
                    abort("randomDevice")
                }
            }

            var PATH_FS = {
                resolve: function () {
                    var resolvedPath = "", resolvedAbsolute = false;
                    for (var i = arguments.length - 1; i >= -1 && !resolvedAbsolute; i--) {
                        var path = i >= 0 ? arguments[i] : FS.cwd();
                        if (typeof path !== "string") {
                            throw new TypeError("Arguments to path.resolve must be strings")
                        } else if (!path) {
                            return ""
                        }
                        resolvedPath = path + "/" + resolvedPath;
                        resolvedAbsolute = path.charAt(0) === "/"
                    }
                    resolvedPath = PATH.normalizeArray(resolvedPath.split("/").filter(function (p) {
                        return !!p
                    }), !resolvedAbsolute).join("/");
                    return (resolvedAbsolute ? "/" : "") + resolvedPath || "."
                }, relative: function (from, to) {
                    from = PATH_FS.resolve(from).substr(1);
                    to = PATH_FS.resolve(to).substr(1);

                    function trim(arr) {
                        var start = 0;
                        for (; start < arr.length; start++) {
                            if (arr[start] !== "") break
                        }
                        var end = arr.length - 1;
                        for (; end >= 0; end--) {
                            if (arr[end] !== "") break
                        }
                        if (start > end) return [];
                        return arr.slice(start, end - start + 1)
                    }

                    var fromParts = trim(from.split("/"));
                    var toParts = trim(to.split("/"));
                    var length = Math.min(fromParts.length, toParts.length);
                    var samePartsLength = length;
                    for (var i = 0; i < length; i++) {
                        if (fromParts[i] !== toParts[i]) {
                            samePartsLength = i;
                            break
                        }
                    }
                    var outputParts = [];
                    for (var i = samePartsLength; i < fromParts.length; i++) {
                        outputParts.push("..")
                    }
                    outputParts = outputParts.concat(toParts.slice(samePartsLength));
                    return outputParts.join("/")
                }
            };
            var TTY = {
                ttys: [], init: function () {
                }, shutdown: function () {
                }, register: function (dev, ops) {
                    TTY.ttys[dev] = {input: [], output: [], ops: ops};
                    FS.registerDevice(dev, TTY.stream_ops)
                }, stream_ops: {
                    open: function (stream) {
                        var tty = TTY.ttys[stream.node.rdev];
                        if (!tty) {
                            throw new FS.ErrnoError(43)
                        }
                        stream.tty = tty;
                        stream.seekable = false
                    }, close: function (stream) {
                        stream.tty.ops.flush(stream.tty)
                    }, flush: function (stream) {
                        stream.tty.ops.flush(stream.tty)
                    }, read: function (stream, buffer, offset, length, pos) {
                        if (!stream.tty || !stream.tty.ops.get_char) {
                            throw new FS.ErrnoError(60)
                        }
                        var bytesRead = 0;
                        for (var i = 0; i < length; i++) {
                            var result;
                            try {
                                result = stream.tty.ops.get_char(stream.tty)
                            } catch (e) {
                                throw new FS.ErrnoError(29)
                            }
                            if (result === undefined && bytesRead === 0) {
                                throw new FS.ErrnoError(6)
                            }
                            if (result === null || result === undefined) break;
                            bytesRead++;
                            buffer[offset + i] = result
                        }
                        if (bytesRead) {
                            stream.node.timestamp = Date.now()
                        }
                        return bytesRead
                    }, write: function (stream, buffer, offset, length, pos) {
                        if (!stream.tty || !stream.tty.ops.put_char) {
                            throw new FS.ErrnoError(60)
                        }
                        try {
                            for (var i = 0; i < length; i++) {
                                stream.tty.ops.put_char(stream.tty, buffer[offset + i])
                            }
                        } catch (e) {
                            throw new FS.ErrnoError(29)
                        }
                        if (length) {
                            stream.node.timestamp = Date.now()
                        }
                        return i
                    }
                }, default_tty_ops: {
                    get_char: function (tty) {
                        if (!tty.input.length) {
                            var result = null;
                            if (ENVIRONMENT_IS_NODE) {
                                var BUFSIZE = 256;
                                var buf = Buffer.alloc(BUFSIZE);
                                var bytesRead = 0;
                                try {
                                    bytesRead = nodeFS.readSync(process.stdin.fd, buf, 0, BUFSIZE, null)
                                } catch (e) {
                                    if (e.toString().includes("EOF")) bytesRead = 0; else throw e
                                }
                                if (bytesRead > 0) {
                                    result = buf.slice(0, bytesRead).toString("utf-8")
                                } else {
                                    result = null
                                }
                            } else if (typeof window != "undefined" && typeof window.prompt == "function") {
                                result = window.prompt("Input: ");
                                if (result !== null) {
                                    result += "\n"
                                }
                            } else if (typeof readline == "function") {
                                result = readline();
                                if (result !== null) {
                                    result += "\n"
                                }
                            }
                            if (!result) {
                                return null
                            }
                            tty.input = intArrayFromString(result, true)
                        }
                        return tty.input.shift()
                    }, put_char: function (tty, val) {
                        if (val === null || val === 10) {
                            out(UTF8ArrayToString(tty.output, 0));
                            tty.output = []
                        } else {
                            if (val != 0) tty.output.push(val)
                        }
                    }, flush: function (tty) {
                        if (tty.output && tty.output.length > 0) {
                            out(UTF8ArrayToString(tty.output, 0));
                            tty.output = []
                        }
                    }
                }, default_tty1_ops: {
                    put_char: function (tty, val) {
                        if (val === null || val === 10) {
                            err(UTF8ArrayToString(tty.output, 0));
                            tty.output = []
                        } else {
                            if (val != 0) tty.output.push(val)
                        }
                    }, flush: function (tty) {
                        if (tty.output && tty.output.length > 0) {
                            err(UTF8ArrayToString(tty.output, 0));
                            tty.output = []
                        }
                    }
                }
            };

            function mmapAlloc(size) {
                abort()
            }

            var MEMFS = {
                ops_table: null, mount: function (mount) {
                    return MEMFS.createNode(null, "/", 16384 | 511, 0)
                }, createNode: function (parent, name, mode, dev) {
                    if (FS.isBlkdev(mode) || FS.isFIFO(mode)) {
                        throw new FS.ErrnoError(63)
                    }
                    if (!MEMFS.ops_table) {
                        MEMFS.ops_table = {
                            dir: {
                                node: {
                                    getattr: MEMFS.node_ops.getattr,
                                    setattr: MEMFS.node_ops.setattr,
                                    lookup: MEMFS.node_ops.lookup,
                                    mknod: MEMFS.node_ops.mknod,
                                    rename: MEMFS.node_ops.rename,
                                    unlink: MEMFS.node_ops.unlink,
                                    rmdir: MEMFS.node_ops.rmdir,
                                    readdir: MEMFS.node_ops.readdir,
                                    symlink: MEMFS.node_ops.symlink
                                }, stream: {llseek: MEMFS.stream_ops.llseek}
                            },
                            file: {
                                node: {getattr: MEMFS.node_ops.getattr, setattr: MEMFS.node_ops.setattr},
                                stream: {
                                    llseek: MEMFS.stream_ops.llseek,
                                    read: MEMFS.stream_ops.read,
                                    write: MEMFS.stream_ops.write,
                                    allocate: MEMFS.stream_ops.allocate,
                                    mmap: MEMFS.stream_ops.mmap,
                                    msync: MEMFS.stream_ops.msync
                                }
                            },
                            link: {
                                node: {
                                    getattr: MEMFS.node_ops.getattr,
                                    setattr: MEMFS.node_ops.setattr,
                                    readlink: MEMFS.node_ops.readlink
                                }, stream: {}
                            },
                            chrdev: {
                                node: {getattr: MEMFS.node_ops.getattr, setattr: MEMFS.node_ops.setattr},
                                stream: FS.chrdev_stream_ops
                            }
                        }
                    }
                    var node = FS.createNode(parent, name, mode, dev);
                    if (FS.isDir(node.mode)) {
                        node.node_ops = MEMFS.ops_table.dir.node;
                        node.stream_ops = MEMFS.ops_table.dir.stream;
                        node.contents = {}
                    } else if (FS.isFile(node.mode)) {
                        node.node_ops = MEMFS.ops_table.file.node;
                        node.stream_ops = MEMFS.ops_table.file.stream;
                        node.usedBytes = 0;
                        node.contents = null
                    } else if (FS.isLink(node.mode)) {
                        node.node_ops = MEMFS.ops_table.link.node;
                        node.stream_ops = MEMFS.ops_table.link.stream
                    } else if (FS.isChrdev(node.mode)) {
                        node.node_ops = MEMFS.ops_table.chrdev.node;
                        node.stream_ops = MEMFS.ops_table.chrdev.stream
                    }
                    node.timestamp = Date.now();
                    if (parent) {
                        parent.contents[name] = node;
                        parent.timestamp = node.timestamp
                    }
                    return node
                }, getFileDataAsTypedArray: function (node) {
                    if (!node.contents) return new Uint8Array(0);
                    if (node.contents.subarray) return node.contents.subarray(0, node.usedBytes);
                    return new Uint8Array(node.contents)
                }, expandFileStorage: function (node, newCapacity) {
                    var prevCapacity = node.contents ? node.contents.length : 0;
                    if (prevCapacity >= newCapacity) return;
                    var CAPACITY_DOUBLING_MAX = 1024 * 1024;
                    newCapacity = Math.max(newCapacity, prevCapacity * (prevCapacity < CAPACITY_DOUBLING_MAX ? 2 : 1.125) >>> 0);
                    if (prevCapacity != 0) newCapacity = Math.max(newCapacity, 256);
                    var oldContents = node.contents;
                    node.contents = new Uint8Array(newCapacity);
                    if (node.usedBytes > 0) node.contents.set(oldContents.subarray(0, node.usedBytes), 0)
                }, resizeFileStorage: function (node, newSize) {
                    if (node.usedBytes == newSize) return;
                    if (newSize == 0) {
                        node.contents = null;
                        node.usedBytes = 0
                    } else {
                        var oldContents = node.contents;
                        node.contents = new Uint8Array(newSize);
                        if (oldContents) {
                            node.contents.set(oldContents.subarray(0, Math.min(newSize, node.usedBytes)))
                        }
                        node.usedBytes = newSize
                    }
                }, node_ops: {
                    getattr: function (node) {
                        var attr = {};
                        attr.dev = FS.isChrdev(node.mode) ? node.id : 1;
                        attr.ino = node.id;
                        attr.mode = node.mode;
                        attr.nlink = 1;
                        attr.uid = 0;
                        attr.gid = 0;
                        attr.rdev = node.rdev;
                        if (FS.isDir(node.mode)) {
                            attr.size = 4096
                        } else if (FS.isFile(node.mode)) {
                            attr.size = node.usedBytes
                        } else if (FS.isLink(node.mode)) {
                            attr.size = node.link.length
                        } else {
                            attr.size = 0
                        }
                        attr.atime = new Date(node.timestamp);
                        attr.mtime = new Date(node.timestamp);
                        attr.ctime = new Date(node.timestamp);
                        attr.blksize = 4096;
                        attr.blocks = Math.ceil(attr.size / attr.blksize);
                        return attr
                    }, setattr: function (node, attr) {
                        if (attr.mode !== undefined) {
                            node.mode = attr.mode
                        }
                        if (attr.timestamp !== undefined) {
                            node.timestamp = attr.timestamp
                        }
                        if (attr.size !== undefined) {
                            MEMFS.resizeFileStorage(node, attr.size)
                        }
                    }, lookup: function (parent, name) {
                        throw FS.genericErrors[44]
                    }, mknod: function (parent, name, mode, dev) {
                        return MEMFS.createNode(parent, name, mode, dev)
                    }, rename: function (old_node, new_dir, new_name) {
                        if (FS.isDir(old_node.mode)) {
                            var new_node;
                            try {
                                new_node = FS.lookupNode(new_dir, new_name)
                            } catch (e) {
                            }
                            if (new_node) {
                                for (var i in new_node.contents) {
                                    throw new FS.ErrnoError(55)
                                }
                            }
                        }
                        delete old_node.parent.contents[old_node.name];
                        old_node.parent.timestamp = Date.now();
                        old_node.name = new_name;
                        new_dir.contents[new_name] = old_node;
                        new_dir.timestamp = old_node.parent.timestamp;
                        old_node.parent = new_dir
                    }, unlink: function (parent, name) {
                        delete parent.contents[name];
                        parent.timestamp = Date.now()
                    }, rmdir: function (parent, name) {
                        var node = FS.lookupNode(parent, name);
                        for (var i in node.contents) {
                            throw new FS.ErrnoError(55)
                        }
                        delete parent.contents[name];
                        parent.timestamp = Date.now()
                    }, readdir: function (node) {
                        var entries = [".", ".."];
                        for (var key in node.contents) {
                            if (!node.contents.hasOwnProperty(key)) {
                                continue
                            }
                            entries.push(key)
                        }
                        return entries
                    }, symlink: function (parent, newname, oldpath) {
                        var node = MEMFS.createNode(parent, newname, 511 | 40960, 0);
                        node.link = oldpath;
                        return node
                    }, readlink: function (node) {
                        if (!FS.isLink(node.mode)) {
                            throw new FS.ErrnoError(28)
                        }
                        return node.link
                    }
                }, stream_ops: {
                    read: function (stream, buffer, offset, length, position) {
                        var contents = stream.node.contents;
                        if (position >= stream.node.usedBytes) return 0;
                        var size = Math.min(stream.node.usedBytes - position, length);
                        if (size > 8 && contents.subarray) {
                            buffer.set(contents.subarray(position, position + size), offset)
                        } else {
                            for (var i = 0; i < size; i++) buffer[offset + i] = contents[position + i]
                        }
                        return size
                    }, write: function (stream, buffer, offset, length, position, canOwn) {
                        if (buffer.buffer === HEAP8.buffer) {
                            canOwn = false
                        }
                        if (!length) return 0;
                        var node = stream.node;
                        node.timestamp = Date.now();
                        if (buffer.subarray && (!node.contents || node.contents.subarray)) {
                            if (canOwn) {
                                node.contents = buffer.subarray(offset, offset + length);
                                node.usedBytes = length;
                                return length
                            } else if (node.usedBytes === 0 && position === 0) {
                                node.contents = buffer.slice(offset, offset + length);
                                node.usedBytes = length;
                                return length
                            } else if (position + length <= node.usedBytes) {
                                node.contents.set(buffer.subarray(offset, offset + length), position);
                                return length
                            }
                        }
                        MEMFS.expandFileStorage(node, position + length);
                        if (node.contents.subarray && buffer.subarray) {
                            node.contents.set(buffer.subarray(offset, offset + length), position)
                        } else {
                            for (var i = 0; i < length; i++) {
                                node.contents[position + i] = buffer[offset + i]
                            }
                        }
                        node.usedBytes = Math.max(node.usedBytes, position + length);
                        return length
                    }, llseek: function (stream, offset, whence) {
                        var position = offset;
                        if (whence === 1) {
                            position += stream.position
                        } else if (whence === 2) {
                            if (FS.isFile(stream.node.mode)) {
                                position += stream.node.usedBytes
                            }
                        }
                        if (position < 0) {
                            throw new FS.ErrnoError(28)
                        }
                        return position
                    }, allocate: function (stream, offset, length) {
                        MEMFS.expandFileStorage(stream.node, offset + length);
                        stream.node.usedBytes = Math.max(stream.node.usedBytes, offset + length)
                    }, mmap: function (stream, address, length, position, prot, flags) {
                        if (address !== 0) {
                            throw new FS.ErrnoError(28)
                        }
                        if (!FS.isFile(stream.node.mode)) {
                            throw new FS.ErrnoError(43)
                        }
                        var ptr;
                        var allocated;
                        var contents = stream.node.contents;
                        if (!(flags & 2) && contents.buffer === buffer) {
                            allocated = false;
                            ptr = contents.byteOffset
                        } else {
                            if (position > 0 || position + length < contents.length) {
                                if (contents.subarray) {
                                    contents = contents.subarray(position, position + length)
                                } else {
                                    contents = Array.prototype.slice.call(contents, position, position + length)
                                }
                            }
                            allocated = true;
                            ptr = mmapAlloc(length);
                            if (!ptr) {
                                throw new FS.ErrnoError(48)
                            }
                            HEAP8.set(contents, ptr)
                        }
                        return {ptr: ptr, allocated: allocated}
                    }, msync: function (stream, buffer, offset, length, mmapFlags) {
                        if (!FS.isFile(stream.node.mode)) {
                            throw new FS.ErrnoError(43)
                        }
                        if (mmapFlags & 2) {
                            return 0
                        }
                        var bytesWritten = MEMFS.stream_ops.write(stream, buffer, 0, length, offset, false);
                        return 0
                    }
                }
            };

            function asyncLoad(url, onload, onerror, noRunDep) {
                var dep = !noRunDep ? getUniqueRunDependency("al " + url) : "";
                readAsync(url, function (arrayBuffer) {
                    assert(arrayBuffer, 'Loading data file "' + url + '" failed (no arrayBuffer).');
                    onload(new Uint8Array(arrayBuffer));
                    if (dep) removeRunDependency(dep)
                }, function (event) {
                    if (onerror) {
                        onerror()
                    } else {
                        throw'Loading data file "' + url + '" failed.'
                    }
                });
                if (dep) addRunDependency(dep)
            }

            var FS = {
                root: null,
                mounts: [],
                devices: {},
                streams: [],
                nextInode: 1,
                nameTable: null,
                currentPath: "/",
                initialized: false,
                ignorePermissions: true,
                trackingDelegate: {},
                tracking: {openFlags: {READ: 1, WRITE: 2}},
                ErrnoError: null,
                genericErrors: {},
                filesystems: null,
                syncFSRequests: 0,
                lookupPath: function (path, opts) {
                    path = PATH_FS.resolve(FS.cwd(), path);
                    opts = opts || {};
                    if (!path) return {path: "", node: null};
                    var defaults = {follow_mount: true, recurse_count: 0};
                    for (var key in defaults) {
                        if (opts[key] === undefined) {
                            opts[key] = defaults[key]
                        }
                    }
                    if (opts.recurse_count > 8) {
                        throw new FS.ErrnoError(32)
                    }
                    var parts = PATH.normalizeArray(path.split("/").filter(function (p) {
                        return !!p
                    }), false);
                    var current = FS.root;
                    var current_path = "/";
                    for (var i = 0; i < parts.length; i++) {
                        var islast = i === parts.length - 1;
                        if (islast && opts.parent) {
                            break
                        }
                        current = FS.lookupNode(current, parts[i]);
                        current_path = PATH.join2(current_path, parts[i]);
                        if (FS.isMountpoint(current)) {
                            if (!islast || islast && opts.follow_mount) {
                                current = current.mounted.root
                            }
                        }
                        if (!islast || opts.follow) {
                            var count = 0;
                            while (FS.isLink(current.mode)) {
                                var link = FS.readlink(current_path);
                                current_path = PATH_FS.resolve(PATH.dirname(current_path), link);
                                var lookup = FS.lookupPath(current_path, {recurse_count: opts.recurse_count});
                                current = lookup.node;
                                if (count++ > 40) {
                                    throw new FS.ErrnoError(32)
                                }
                            }
                        }
                    }
                    return {path: current_path, node: current}
                },
                getPath: function (node) {
                    var path;
                    while (true) {
                        if (FS.isRoot(node)) {
                            var mount = node.mount.mountpoint;
                            if (!path) return mount;
                            return mount[mount.length - 1] !== "/" ? mount + "/" + path : mount + path
                        }
                        path = path ? node.name + "/" + path : node.name;
                        node = node.parent
                    }
                },
                hashName: function (parentid, name) {
                    var hash = 0;
                    for (var i = 0; i < name.length; i++) {
                        hash = (hash << 5) - hash + name.charCodeAt(i) | 0
                    }
                    return (parentid + hash >>> 0) % FS.nameTable.length
                },
                hashAddNode: function (node) {
                    var hash = FS.hashName(node.parent.id, node.name);
                    node.name_next = FS.nameTable[hash];
                    FS.nameTable[hash] = node
                },
                hashRemoveNode: function (node) {
                    var hash = FS.hashName(node.parent.id, node.name);
                    if (FS.nameTable[hash] === node) {
                        FS.nameTable[hash] = node.name_next
                    } else {
                        var current = FS.nameTable[hash];
                        while (current) {
                            if (current.name_next === node) {
                                current.name_next = node.name_next;
                                break
                            }
                            current = current.name_next
                        }
                    }
                },
                lookupNode: function (parent, name) {
                    var errCode = FS.mayLookup(parent);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode, parent)
                    }
                    var hash = FS.hashName(parent.id, name);
                    for (var node = FS.nameTable[hash]; node; node = node.name_next) {
                        var nodeName = node.name;
                        if (node.parent.id === parent.id && nodeName === name) {
                            return node
                        }
                    }
                    return FS.lookup(parent, name)
                },
                createNode: function (parent, name, mode, rdev) {
                    var node = new FS.FSNode(parent, name, mode, rdev);
                    FS.hashAddNode(node);
                    return node
                },
                destroyNode: function (node) {
                    FS.hashRemoveNode(node)
                },
                isRoot: function (node) {
                    return node === node.parent
                },
                isMountpoint: function (node) {
                    return !!node.mounted
                },
                isFile: function (mode) {
                    return (mode & 61440) === 32768
                },
                isDir: function (mode) {
                    return (mode & 61440) === 16384
                },
                isLink: function (mode) {
                    return (mode & 61440) === 40960
                },
                isChrdev: function (mode) {
                    return (mode & 61440) === 8192
                },
                isBlkdev: function (mode) {
                    return (mode & 61440) === 24576
                },
                isFIFO: function (mode) {
                    return (mode & 61440) === 4096
                },
                isSocket: function (mode) {
                    return (mode & 49152) === 49152
                },
                flagModes: {"r": 0, "r+": 2, "w": 577, "w+": 578, "a": 1089, "a+": 1090},
                modeStringToFlags: function (str) {
                    var flags = FS.flagModes[str];
                    if (typeof flags === "undefined") {
                        throw new Error("Unknown file open mode: " + str)
                    }
                    return flags
                },
                flagsToPermissionString: function (flag) {
                    var perms = ["r", "w", "rw"][flag & 3];
                    if (flag & 512) {
                        perms += "w"
                    }
                    return perms
                },
                nodePermissions: function (node, perms) {
                    if (FS.ignorePermissions) {
                        return 0
                    }
                    if (perms.includes("r") && !(node.mode & 292)) {
                        return 2
                    } else if (perms.includes("w") && !(node.mode & 146)) {
                        return 2
                    } else if (perms.includes("x") && !(node.mode & 73)) {
                        return 2
                    }
                    return 0
                },
                mayLookup: function (dir) {
                    var errCode = FS.nodePermissions(dir, "x");
                    if (errCode) return errCode;
                    if (!dir.node_ops.lookup) return 2;
                    return 0
                },
                mayCreate: function (dir, name) {
                    try {
                        var node = FS.lookupNode(dir, name);
                        return 20
                    } catch (e) {
                    }
                    return FS.nodePermissions(dir, "wx")
                },
                mayDelete: function (dir, name, isdir) {
                    var node;
                    try {
                        node = FS.lookupNode(dir, name)
                    } catch (e) {
                        return e.errno
                    }
                    var errCode = FS.nodePermissions(dir, "wx");
                    if (errCode) {
                        return errCode
                    }
                    if (isdir) {
                        if (!FS.isDir(node.mode)) {
                            return 54
                        }
                        if (FS.isRoot(node) || FS.getPath(node) === FS.cwd()) {
                            return 10
                        }
                    } else {
                        if (FS.isDir(node.mode)) {
                            return 31
                        }
                    }
                    return 0
                },
                mayOpen: function (node, flags) {
                    if (!node) {
                        return 44
                    }
                    if (FS.isLink(node.mode)) {
                        return 32
                    } else if (FS.isDir(node.mode)) {
                        if (FS.flagsToPermissionString(flags) !== "r" || flags & 512) {
                            return 31
                        }
                    }
                    return FS.nodePermissions(node, FS.flagsToPermissionString(flags))
                },
                MAX_OPEN_FDS: 4096,
                nextfd: function (fd_start, fd_end) {
                    fd_start = fd_start || 0;
                    fd_end = fd_end || FS.MAX_OPEN_FDS;
                    for (var fd = fd_start; fd <= fd_end; fd++) {
                        if (!FS.streams[fd]) {
                            return fd
                        }
                    }
                    throw new FS.ErrnoError(33)
                },
                getStream: function (fd) {
                    return FS.streams[fd]
                },
                createStream: function (stream, fd_start, fd_end) {
                    if (!FS.FSStream) {
                        FS.FSStream = function () {
                        };
                        FS.FSStream.prototype = {
                            object: {
                                get: function () {
                                    return this.node
                                }, set: function (val) {
                                    this.node = val
                                }
                            }, isRead: {
                                get: function () {
                                    return (this.flags & 2097155) !== 1
                                }
                            }, isWrite: {
                                get: function () {
                                    return (this.flags & 2097155) !== 0
                                }
                            }, isAppend: {
                                get: function () {
                                    return this.flags & 1024
                                }
                            }
                        }
                    }
                    var newStream = new FS.FSStream;
                    for (var p in stream) {
                        newStream[p] = stream[p]
                    }
                    stream = newStream;
                    var fd = FS.nextfd(fd_start, fd_end);
                    stream.fd = fd;
                    FS.streams[fd] = stream;
                    return stream
                },
                closeStream: function (fd) {
                    FS.streams[fd] = null
                },
                chrdev_stream_ops: {
                    open: function (stream) {
                        var device = FS.getDevice(stream.node.rdev);
                        stream.stream_ops = device.stream_ops;
                        if (stream.stream_ops.open) {
                            stream.stream_ops.open(stream)
                        }
                    }, llseek: function () {
                        throw new FS.ErrnoError(70)
                    }
                },
                major: function (dev) {
                    return dev >> 8
                },
                minor: function (dev) {
                    return dev & 255
                },
                makedev: function (ma, mi) {
                    return ma << 8 | mi
                },
                registerDevice: function (dev, ops) {
                    FS.devices[dev] = {stream_ops: ops}
                },
                getDevice: function (dev) {
                    return FS.devices[dev]
                },
                getMounts: function (mount) {
                    var mounts = [];
                    var check = [mount];
                    while (check.length) {
                        var m = check.pop();
                        mounts.push(m);
                        check.push.apply(check, m.mounts)
                    }
                    return mounts
                },
                syncfs: function (populate, callback) {
                    if (typeof populate === "function") {
                        callback = populate;
                        populate = false
                    }
                    FS.syncFSRequests++;
                    if (FS.syncFSRequests > 1) {
                        err("warning: " + FS.syncFSRequests + " FS.syncfs operations in flight at once, probably just doing extra work")
                    }
                    var mounts = FS.getMounts(FS.root.mount);
                    var completed = 0;

                    function doCallback(errCode) {
                        FS.syncFSRequests--;
                        return callback(errCode)
                    }

                    function done(errCode) {
                        if (errCode) {
                            if (!done.errored) {
                                done.errored = true;
                                return doCallback(errCode)
                            }
                            return
                        }
                        if (++completed >= mounts.length) {
                            doCallback(null)
                        }
                    }

                    mounts.forEach(function (mount) {
                        if (!mount.type.syncfs) {
                            return done(null)
                        }
                        mount.type.syncfs(mount, populate, done)
                    })
                },
                mount: function (type, opts, mountpoint) {
                    var root = mountpoint === "/";
                    var pseudo = !mountpoint;
                    var node;
                    if (root && FS.root) {
                        throw new FS.ErrnoError(10)
                    } else if (!root && !pseudo) {
                        var lookup = FS.lookupPath(mountpoint, {follow_mount: false});
                        mountpoint = lookup.path;
                        node = lookup.node;
                        if (FS.isMountpoint(node)) {
                            throw new FS.ErrnoError(10)
                        }
                        if (!FS.isDir(node.mode)) {
                            throw new FS.ErrnoError(54)
                        }
                    }
                    var mount = {type: type, opts: opts, mountpoint: mountpoint, mounts: []};
                    var mountRoot = type.mount(mount);
                    mountRoot.mount = mount;
                    mount.root = mountRoot;
                    if (root) {
                        FS.root = mountRoot
                    } else if (node) {
                        node.mounted = mount;
                        if (node.mount) {
                            node.mount.mounts.push(mount)
                        }
                    }
                    return mountRoot
                },
                unmount: function (mountpoint) {
                    var lookup = FS.lookupPath(mountpoint, {follow_mount: false});
                    if (!FS.isMountpoint(lookup.node)) {
                        throw new FS.ErrnoError(28)
                    }
                    var node = lookup.node;
                    var mount = node.mounted;
                    var mounts = FS.getMounts(mount);
                    Object.keys(FS.nameTable).forEach(function (hash) {
                        var current = FS.nameTable[hash];
                        while (current) {
                            var next = current.name_next;
                            if (mounts.includes(current.mount)) {
                                FS.destroyNode(current)
                            }
                            current = next
                        }
                    });
                    node.mounted = null;
                    var idx = node.mount.mounts.indexOf(mount);
                    node.mount.mounts.splice(idx, 1)
                },
                lookup: function (parent, name) {
                    return parent.node_ops.lookup(parent, name)
                },
                mknod: function (path, mode, dev) {
                    var lookup = FS.lookupPath(path, {parent: true});
                    var parent = lookup.node;
                    var name = PATH.basename(path);
                    if (!name || name === "." || name === "..") {
                        throw new FS.ErrnoError(28)
                    }
                    var errCode = FS.mayCreate(parent, name);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    if (!parent.node_ops.mknod) {
                        throw new FS.ErrnoError(63)
                    }
                    return parent.node_ops.mknod(parent, name, mode, dev)
                },
                create: function (path, mode) {
                    mode = mode !== undefined ? mode : 438;
                    mode &= 4095;
                    mode |= 32768;
                    return FS.mknod(path, mode, 0)
                },
                mkdir: function (path, mode) {
                    mode = mode !== undefined ? mode : 511;
                    mode &= 511 | 512;
                    mode |= 16384;
                    return FS.mknod(path, mode, 0)
                },
                mkdirTree: function (path, mode) {
                    var dirs = path.split("/");
                    var d = "";
                    for (var i = 0; i < dirs.length; ++i) {
                        if (!dirs[i]) continue;
                        d += "/" + dirs[i];
                        try {
                            FS.mkdir(d, mode)
                        } catch (e) {
                            if (e.errno != 20) throw e
                        }
                    }
                },
                mkdev: function (path, mode, dev) {
                    if (typeof dev === "undefined") {
                        dev = mode;
                        mode = 438
                    }
                    mode |= 8192;
                    return FS.mknod(path, mode, dev)
                },
                symlink: function (oldpath, newpath) {
                    if (!PATH_FS.resolve(oldpath)) {
                        throw new FS.ErrnoError(44)
                    }
                    var lookup = FS.lookupPath(newpath, {parent: true});
                    var parent = lookup.node;
                    if (!parent) {
                        throw new FS.ErrnoError(44)
                    }
                    var newname = PATH.basename(newpath);
                    var errCode = FS.mayCreate(parent, newname);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    if (!parent.node_ops.symlink) {
                        throw new FS.ErrnoError(63)
                    }
                    return parent.node_ops.symlink(parent, newname, oldpath)
                },
                rename: function (old_path, new_path) {
                    var old_dirname = PATH.dirname(old_path);
                    var new_dirname = PATH.dirname(new_path);
                    var old_name = PATH.basename(old_path);
                    var new_name = PATH.basename(new_path);
                    var lookup, old_dir, new_dir;
                    lookup = FS.lookupPath(old_path, {parent: true});
                    old_dir = lookup.node;
                    lookup = FS.lookupPath(new_path, {parent: true});
                    new_dir = lookup.node;
                    if (!old_dir || !new_dir) throw new FS.ErrnoError(44);
                    if (old_dir.mount !== new_dir.mount) {
                        throw new FS.ErrnoError(75)
                    }
                    var old_node = FS.lookupNode(old_dir, old_name);
                    var relative = PATH_FS.relative(old_path, new_dirname);
                    if (relative.charAt(0) !== ".") {
                        throw new FS.ErrnoError(28)
                    }
                    relative = PATH_FS.relative(new_path, old_dirname);
                    if (relative.charAt(0) !== ".") {
                        throw new FS.ErrnoError(55)
                    }
                    var new_node;
                    try {
                        new_node = FS.lookupNode(new_dir, new_name)
                    } catch (e) {
                    }
                    if (old_node === new_node) {
                        return
                    }
                    var isdir = FS.isDir(old_node.mode);
                    var errCode = FS.mayDelete(old_dir, old_name, isdir);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    errCode = new_node ? FS.mayDelete(new_dir, new_name, isdir) : FS.mayCreate(new_dir, new_name);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    if (!old_dir.node_ops.rename) {
                        throw new FS.ErrnoError(63)
                    }
                    if (FS.isMountpoint(old_node) || new_node && FS.isMountpoint(new_node)) {
                        throw new FS.ErrnoError(10)
                    }
                    if (new_dir !== old_dir) {
                        errCode = FS.nodePermissions(old_dir, "w");
                        if (errCode) {
                            throw new FS.ErrnoError(errCode)
                        }
                    }
                    try {
                        if (FS.trackingDelegate["willMovePath"]) {
                            FS.trackingDelegate["willMovePath"](old_path, new_path)
                        }
                    } catch (e) {
                        err("FS.trackingDelegate['willMovePath']('" + old_path + "', '" + new_path + "') threw an exception: " + e.message)
                    }
                    FS.hashRemoveNode(old_node);
                    try {
                        old_dir.node_ops.rename(old_node, new_dir, new_name)
                    } catch (e) {
                        throw e
                    } finally {
                        FS.hashAddNode(old_node)
                    }
                    try {
                        if (FS.trackingDelegate["onMovePath"]) FS.trackingDelegate["onMovePath"](old_path, new_path)
                    } catch (e) {
                        err("FS.trackingDelegate['onMovePath']('" + old_path + "', '" + new_path + "') threw an exception: " + e.message)
                    }
                },
                rmdir: function (path) {
                    var lookup = FS.lookupPath(path, {parent: true});
                    var parent = lookup.node;
                    var name = PATH.basename(path);
                    var node = FS.lookupNode(parent, name);
                    var errCode = FS.mayDelete(parent, name, true);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    if (!parent.node_ops.rmdir) {
                        throw new FS.ErrnoError(63)
                    }
                    if (FS.isMountpoint(node)) {
                        throw new FS.ErrnoError(10)
                    }
                    try {
                        if (FS.trackingDelegate["willDeletePath"]) {
                            FS.trackingDelegate["willDeletePath"](path)
                        }
                    } catch (e) {
                        err("FS.trackingDelegate['willDeletePath']('" + path + "') threw an exception: " + e.message)
                    }
                    parent.node_ops.rmdir(parent, name);
                    FS.destroyNode(node);
                    try {
                        if (FS.trackingDelegate["onDeletePath"]) FS.trackingDelegate["onDeletePath"](path)
                    } catch (e) {
                        err("FS.trackingDelegate['onDeletePath']('" + path + "') threw an exception: " + e.message)
                    }
                },
                readdir: function (path) {
                    var lookup = FS.lookupPath(path, {follow: true});
                    var node = lookup.node;
                    if (!node.node_ops.readdir) {
                        throw new FS.ErrnoError(54)
                    }
                    return node.node_ops.readdir(node)
                },
                unlink: function (path) {
                    var lookup = FS.lookupPath(path, {parent: true});
                    var parent = lookup.node;
                    var name = PATH.basename(path);
                    var node = FS.lookupNode(parent, name);
                    var errCode = FS.mayDelete(parent, name, false);
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    if (!parent.node_ops.unlink) {
                        throw new FS.ErrnoError(63)
                    }
                    if (FS.isMountpoint(node)) {
                        throw new FS.ErrnoError(10)
                    }
                    try {
                        if (FS.trackingDelegate["willDeletePath"]) {
                            FS.trackingDelegate["willDeletePath"](path)
                        }
                    } catch (e) {
                        err("FS.trackingDelegate['willDeletePath']('" + path + "') threw an exception: " + e.message)
                    }
                    parent.node_ops.unlink(parent, name);
                    FS.destroyNode(node);
                    try {
                        if (FS.trackingDelegate["onDeletePath"]) FS.trackingDelegate["onDeletePath"](path)
                    } catch (e) {
                        err("FS.trackingDelegate['onDeletePath']('" + path + "') threw an exception: " + e.message)
                    }
                },
                readlink: function (path) {
                    var lookup = FS.lookupPath(path);
                    var link = lookup.node;
                    if (!link) {
                        throw new FS.ErrnoError(44)
                    }
                    if (!link.node_ops.readlink) {
                        throw new FS.ErrnoError(28)
                    }
                    return PATH_FS.resolve(FS.getPath(link.parent), link.node_ops.readlink(link))
                },
                stat: function (path, dontFollow) {
                    var lookup = FS.lookupPath(path, {follow: !dontFollow});
                    var node = lookup.node;
                    if (!node) {
                        throw new FS.ErrnoError(44)
                    }
                    if (!node.node_ops.getattr) {
                        throw new FS.ErrnoError(63)
                    }
                    return node.node_ops.getattr(node)
                },
                lstat: function (path) {
                    return FS.stat(path, true)
                },
                chmod: function (path, mode, dontFollow) {
                    var node;
                    if (typeof path === "string") {
                        var lookup = FS.lookupPath(path, {follow: !dontFollow});
                        node = lookup.node
                    } else {
                        node = path
                    }
                    if (!node.node_ops.setattr) {
                        throw new FS.ErrnoError(63)
                    }
                    node.node_ops.setattr(node, {mode: mode & 4095 | node.mode & ~4095, timestamp: Date.now()})
                },
                lchmod: function (path, mode) {
                    FS.chmod(path, mode, true)
                },
                fchmod: function (fd, mode) {
                    var stream = FS.getStream(fd);
                    if (!stream) {
                        throw new FS.ErrnoError(8)
                    }
                    FS.chmod(stream.node, mode)
                },
                chown: function (path, uid, gid, dontFollow) {
                    var node;
                    if (typeof path === "string") {
                        var lookup = FS.lookupPath(path, {follow: !dontFollow});
                        node = lookup.node
                    } else {
                        node = path
                    }
                    if (!node.node_ops.setattr) {
                        throw new FS.ErrnoError(63)
                    }
                    node.node_ops.setattr(node, {timestamp: Date.now()})
                },
                lchown: function (path, uid, gid) {
                    FS.chown(path, uid, gid, true)
                },
                fchown: function (fd, uid, gid) {
                    var stream = FS.getStream(fd);
                    if (!stream) {
                        throw new FS.ErrnoError(8)
                    }
                    FS.chown(stream.node, uid, gid)
                },
                truncate: function (path, len) {
                    if (len < 0) {
                        throw new FS.ErrnoError(28)
                    }
                    var node;
                    if (typeof path === "string") {
                        var lookup = FS.lookupPath(path, {follow: true});
                        node = lookup.node
                    } else {
                        node = path
                    }
                    if (!node.node_ops.setattr) {
                        throw new FS.ErrnoError(63)
                    }
                    if (FS.isDir(node.mode)) {
                        throw new FS.ErrnoError(31)
                    }
                    if (!FS.isFile(node.mode)) {
                        throw new FS.ErrnoError(28)
                    }
                    var errCode = FS.nodePermissions(node, "w");
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    node.node_ops.setattr(node, {size: len, timestamp: Date.now()})
                },
                ftruncate: function (fd, len) {
                    var stream = FS.getStream(fd);
                    if (!stream) {
                        throw new FS.ErrnoError(8)
                    }
                    if ((stream.flags & 2097155) === 0) {
                        throw new FS.ErrnoError(28)
                    }
                    FS.truncate(stream.node, len)
                },
                utime: function (path, atime, mtime) {
                    var lookup = FS.lookupPath(path, {follow: true});
                    var node = lookup.node;
                    node.node_ops.setattr(node, {timestamp: Math.max(atime, mtime)})
                },
                open: function (path, flags, mode, fd_start, fd_end) {
                    if (path === "") {
                        throw new FS.ErrnoError(44)
                    }
                    flags = typeof flags === "string" ? FS.modeStringToFlags(flags) : flags;
                    mode = typeof mode === "undefined" ? 438 : mode;
                    if (flags & 64) {
                        mode = mode & 4095 | 32768
                    } else {
                        mode = 0
                    }
                    var node;
                    if (typeof path === "object") {
                        node = path
                    } else {
                        path = PATH.normalize(path);
                        try {
                            var lookup = FS.lookupPath(path, {follow: !(flags & 131072)});
                            node = lookup.node
                        } catch (e) {
                        }
                    }
                    var created = false;
                    if (flags & 64) {
                        if (node) {
                            if (flags & 128) {
                                throw new FS.ErrnoError(20)
                            }
                        } else {
                            node = FS.mknod(path, mode, 0);
                            created = true
                        }
                    }
                    if (!node) {
                        throw new FS.ErrnoError(44)
                    }
                    if (FS.isChrdev(node.mode)) {
                        flags &= ~512
                    }
                    if (flags & 65536 && !FS.isDir(node.mode)) {
                        throw new FS.ErrnoError(54)
                    }
                    if (!created) {
                        var errCode = FS.mayOpen(node, flags);
                        if (errCode) {
                            throw new FS.ErrnoError(errCode)
                        }
                    }
                    if (flags & 512) {
                        FS.truncate(node, 0)
                    }
                    flags &= ~(128 | 512 | 131072);
                    var stream = FS.createStream({
                        node: node,
                        path: FS.getPath(node),
                        flags: flags,
                        seekable: true,
                        position: 0,
                        stream_ops: node.stream_ops,
                        ungotten: [],
                        error: false
                    }, fd_start, fd_end);
                    if (stream.stream_ops.open) {
                        stream.stream_ops.open(stream)
                    }
                    if (Module["logReadFiles"] && !(flags & 1)) {
                        if (!FS.readFiles) FS.readFiles = {};
                        if (!(path in FS.readFiles)) {
                            FS.readFiles[path] = 1;
                            err("FS.trackingDelegate error on read file: " + path)
                        }
                    }
                    try {
                        if (FS.trackingDelegate["onOpenFile"]) {
                            var trackingFlags = 0;
                            if ((flags & 2097155) !== 1) {
                                trackingFlags |= FS.tracking.openFlags.READ
                            }
                            if ((flags & 2097155) !== 0) {
                                trackingFlags |= FS.tracking.openFlags.WRITE
                            }
                            FS.trackingDelegate["onOpenFile"](path, trackingFlags)
                        }
                    } catch (e) {
                        err("FS.trackingDelegate['onOpenFile']('" + path + "', flags) threw an exception: " + e.message)
                    }
                    return stream
                },
                close: function (stream) {
                    if (FS.isClosed(stream)) {
                        throw new FS.ErrnoError(8)
                    }
                    if (stream.getdents) stream.getdents = null;
                    try {
                        if (stream.stream_ops.close) {
                            stream.stream_ops.close(stream)
                        }
                    } catch (e) {
                        throw e
                    } finally {
                        FS.closeStream(stream.fd)
                    }
                    stream.fd = null
                },
                isClosed: function (stream) {
                    return stream.fd === null
                },
                llseek: function (stream, offset, whence) {
                    if (FS.isClosed(stream)) {
                        throw new FS.ErrnoError(8)
                    }
                    if (!stream.seekable || !stream.stream_ops.llseek) {
                        throw new FS.ErrnoError(70)
                    }
                    if (whence != 0 && whence != 1 && whence != 2) {
                        throw new FS.ErrnoError(28)
                    }
                    stream.position = stream.stream_ops.llseek(stream, offset, whence);
                    stream.ungotten = [];
                    return stream.position
                },
                read: function (stream, buffer, offset, length, position) {
                    if (length < 0 || position < 0) {
                        throw new FS.ErrnoError(28)
                    }
                    if (FS.isClosed(stream)) {
                        throw new FS.ErrnoError(8)
                    }
                    if ((stream.flags & 2097155) === 1) {
                        throw new FS.ErrnoError(8)
                    }
                    if (FS.isDir(stream.node.mode)) {
                        throw new FS.ErrnoError(31)
                    }
                    if (!stream.stream_ops.read) {
                        throw new FS.ErrnoError(28)
                    }
                    var seeking = typeof position !== "undefined";
                    if (!seeking) {
                        position = stream.position
                    } else if (!stream.seekable) {
                        throw new FS.ErrnoError(70)
                    }
                    var bytesRead = stream.stream_ops.read(stream, buffer, offset, length, position);
                    if (!seeking) stream.position += bytesRead;
                    return bytesRead
                },
                write: function (stream, buffer, offset, length, position, canOwn) {
                    if (length < 0 || position < 0) {
                        throw new FS.ErrnoError(28)
                    }
                    if (FS.isClosed(stream)) {
                        throw new FS.ErrnoError(8)
                    }
                    if ((stream.flags & 2097155) === 0) {
                        throw new FS.ErrnoError(8)
                    }
                    if (FS.isDir(stream.node.mode)) {
                        throw new FS.ErrnoError(31)
                    }
                    if (!stream.stream_ops.write) {
                        throw new FS.ErrnoError(28)
                    }
                    if (stream.seekable && stream.flags & 1024) {
                        FS.llseek(stream, 0, 2)
                    }
                    var seeking = typeof position !== "undefined";
                    if (!seeking) {
                        position = stream.position
                    } else if (!stream.seekable) {
                        throw new FS.ErrnoError(70)
                    }
                    var bytesWritten = stream.stream_ops.write(stream, buffer, offset, length, position, canOwn);
                    if (!seeking) stream.position += bytesWritten;
                    try {
                        if (stream.path && FS.trackingDelegate["onWriteToFile"]) FS.trackingDelegate["onWriteToFile"](stream.path)
                    } catch (e) {
                        err("FS.trackingDelegate['onWriteToFile']('" + stream.path + "') threw an exception: " + e.message)
                    }
                    return bytesWritten
                },
                allocate: function (stream, offset, length) {
                    if (FS.isClosed(stream)) {
                        throw new FS.ErrnoError(8)
                    }
                    if (offset < 0 || length <= 0) {
                        throw new FS.ErrnoError(28)
                    }
                    if ((stream.flags & 2097155) === 0) {
                        throw new FS.ErrnoError(8)
                    }
                    if (!FS.isFile(stream.node.mode) && !FS.isDir(stream.node.mode)) {
                        throw new FS.ErrnoError(43)
                    }
                    if (!stream.stream_ops.allocate) {
                        throw new FS.ErrnoError(138)
                    }
                    stream.stream_ops.allocate(stream, offset, length)
                },
                mmap: function (stream, address, length, position, prot, flags) {
                    if ((prot & 2) !== 0 && (flags & 2) === 0 && (stream.flags & 2097155) !== 2) {
                        throw new FS.ErrnoError(2)
                    }
                    if ((stream.flags & 2097155) === 1) {
                        throw new FS.ErrnoError(2)
                    }
                    if (!stream.stream_ops.mmap) {
                        throw new FS.ErrnoError(43)
                    }
                    return stream.stream_ops.mmap(stream, address, length, position, prot, flags)
                },
                msync: function (stream, buffer, offset, length, mmapFlags) {
                    if (!stream || !stream.stream_ops.msync) {
                        return 0
                    }
                    return stream.stream_ops.msync(stream, buffer, offset, length, mmapFlags)
                },
                munmap: function (stream) {
                    return 0
                },
                ioctl: function (stream, cmd, arg) {
                    if (!stream.stream_ops.ioctl) {
                        throw new FS.ErrnoError(59)
                    }
                    return stream.stream_ops.ioctl(stream, cmd, arg)
                },
                readFile: function (path, opts) {
                    opts = opts || {};
                    opts.flags = opts.flags || 0;
                    opts.encoding = opts.encoding || "binary";
                    if (opts.encoding !== "utf8" && opts.encoding !== "binary") {
                        throw new Error('Invalid encoding type "' + opts.encoding + '"')
                    }
                    var ret;
                    var stream = FS.open(path, opts.flags);
                    var stat = FS.stat(path);
                    var length = stat.size;
                    var buf = new Uint8Array(length);
                    FS.read(stream, buf, 0, length, 0);
                    if (opts.encoding === "utf8") {
                        ret = UTF8ArrayToString(buf, 0)
                    } else if (opts.encoding === "binary") {
                        ret = buf
                    }
                    FS.close(stream);
                    return ret
                },
                writeFile: function (path, data, opts) {
                    opts = opts || {};
                    opts.flags = opts.flags || 577;
                    var stream = FS.open(path, opts.flags, opts.mode);
                    if (typeof data === "string") {
                        var buf = new Uint8Array(lengthBytesUTF8(data) + 1);
                        var actualNumBytes = stringToUTF8Array(data, buf, 0, buf.length);
                        FS.write(stream, buf, 0, actualNumBytes, undefined, opts.canOwn)
                    } else if (ArrayBuffer.isView(data)) {
                        FS.write(stream, data, 0, data.byteLength, undefined, opts.canOwn)
                    } else {
                        throw new Error("Unsupported data type")
                    }
                    FS.close(stream)
                },
                cwd: function () {
                    return FS.currentPath
                },
                chdir: function (path) {
                    var lookup = FS.lookupPath(path, {follow: true});
                    if (lookup.node === null) {
                        throw new FS.ErrnoError(44)
                    }
                    if (!FS.isDir(lookup.node.mode)) {
                        throw new FS.ErrnoError(54)
                    }
                    var errCode = FS.nodePermissions(lookup.node, "x");
                    if (errCode) {
                        throw new FS.ErrnoError(errCode)
                    }
                    FS.currentPath = lookup.path
                },
                createDefaultDirectories: function () {
                    FS.mkdir("/tmp");
                    FS.mkdir("/home");
                    FS.mkdir("/home/web_user")
                },
                createDefaultDevices: function () {
                    FS.mkdir("/dev");
                    FS.registerDevice(FS.makedev(1, 3), {
                        read: function () {
                            return 0
                        }, write: function (stream, buffer, offset, length, pos) {
                            return length
                        }
                    });
                    FS.mkdev("/dev/null", FS.makedev(1, 3));
                    TTY.register(FS.makedev(5, 0), TTY.default_tty_ops);
                    TTY.register(FS.makedev(6, 0), TTY.default_tty1_ops);
                    FS.mkdev("/dev/tty", FS.makedev(5, 0));
                    FS.mkdev("/dev/tty1", FS.makedev(6, 0));
                    var random_device = getRandomDevice();
                    FS.createDevice("/dev", "random", random_device);
                    FS.createDevice("/dev", "urandom", random_device);
                    FS.mkdir("/dev/shm");
                    FS.mkdir("/dev/shm/tmp")
                },
                createSpecialDirectories: function () {
                    FS.mkdir("/proc");
                    var proc_self = FS.mkdir("/proc/self");
                    FS.mkdir("/proc/self/fd");
                    FS.mount({
                        mount: function () {
                            var node = FS.createNode(proc_self, "fd", 16384 | 511, 73);
                            node.node_ops = {
                                lookup: function (parent, name) {
                                    var fd = +name;
                                    var stream = FS.getStream(fd);
                                    if (!stream) throw new FS.ErrnoError(8);
                                    var ret = {
                                        parent: null,
                                        mount: {mountpoint: "fake"},
                                        node_ops: {
                                            readlink: function () {
                                                return stream.path
                                            }
                                        }
                                    };
                                    ret.parent = ret;
                                    return ret
                                }
                            };
                            return node
                        }
                    }, {}, "/proc/self/fd")
                },
                createStandardStreams: function () {
                    if (Module["stdin"]) {
                        FS.createDevice("/dev", "stdin", Module["stdin"])
                    } else {
                        FS.symlink("/dev/tty", "/dev/stdin")
                    }
                    if (Module["stdout"]) {
                        FS.createDevice("/dev", "stdout", null, Module["stdout"])
                    } else {
                        FS.symlink("/dev/tty", "/dev/stdout")
                    }
                    if (Module["stderr"]) {
                        FS.createDevice("/dev", "stderr", null, Module["stderr"])
                    } else {
                        FS.symlink("/dev/tty1", "/dev/stderr")
                    }
                    var stdin = FS.open("/dev/stdin", 0);
                    var stdout = FS.open("/dev/stdout", 1);
                    var stderr = FS.open("/dev/stderr", 1)
                },
                ensureErrnoError: function () {
                    if (FS.ErrnoError) return;
                    FS.ErrnoError = function ErrnoError(errno, node) {
                        this.node = node;
                        this.setErrno = function (errno) {
                            this.errno = errno
                        };
                        this.setErrno(errno);
                        this.message = "FS error"
                    };
                    FS.ErrnoError.prototype = new Error;
                    FS.ErrnoError.prototype.constructor = FS.ErrnoError;
                    [44].forEach(function (code) {
                        FS.genericErrors[code] = new FS.ErrnoError(code);
                        FS.genericErrors[code].stack = "<generic error, no stack>"
                    })
                },
                staticInit: function () {
                    FS.ensureErrnoError();
                    FS.nameTable = new Array(4096);
                    FS.mount(MEMFS, {}, "/");
                    FS.createDefaultDirectories();
                    FS.createDefaultDevices();
                    FS.createSpecialDirectories();
                    FS.filesystems = {"MEMFS": MEMFS}
                },
                init: function (input, output, error) {
                    FS.init.initialized = true;
                    FS.ensureErrnoError();
                    Module["stdin"] = input || Module["stdin"];
                    Module["stdout"] = output || Module["stdout"];
                    Module["stderr"] = error || Module["stderr"];
                    FS.createStandardStreams()
                },
                quit: function () {
                    FS.init.initialized = false;
                    var fflush = Module["_fflush"];
                    if (fflush) fflush(0);
                    for (var i = 0; i < FS.streams.length; i++) {
                        var stream = FS.streams[i];
                        if (!stream) {
                            continue
                        }
                        FS.close(stream)
                    }
                },
                getMode: function (canRead, canWrite) {
                    var mode = 0;
                    if (canRead) mode |= 292 | 73;
                    if (canWrite) mode |= 146;
                    return mode
                },
                findObject: function (path, dontResolveLastLink) {
                    var ret = FS.analyzePath(path, dontResolveLastLink);
                    if (ret.exists) {
                        return ret.object
                    } else {
                        return null
                    }
                },
                analyzePath: function (path, dontResolveLastLink) {
                    try {
                        var lookup = FS.lookupPath(path, {follow: !dontResolveLastLink});
                        path = lookup.path
                    } catch (e) {
                    }
                    var ret = {
                        isRoot: false,
                        exists: false,
                        error: 0,
                        name: null,
                        path: null,
                        object: null,
                        parentExists: false,
                        parentPath: null,
                        parentObject: null
                    };
                    try {
                        var lookup = FS.lookupPath(path, {parent: true});
                        ret.parentExists = true;
                        ret.parentPath = lookup.path;
                        ret.parentObject = lookup.node;
                        ret.name = PATH.basename(path);
                        lookup = FS.lookupPath(path, {follow: !dontResolveLastLink});
                        ret.exists = true;
                        ret.path = lookup.path;
                        ret.object = lookup.node;
                        ret.name = lookup.node.name;
                        ret.isRoot = lookup.path === "/"
                    } catch (e) {
                        ret.error = e.errno
                    }
                    return ret
                },
                createPath: function (parent, path, canRead, canWrite) {
                    parent = typeof parent === "string" ? parent : FS.getPath(parent);
                    var parts = path.split("/").reverse();
                    while (parts.length) {
                        var part = parts.pop();
                        if (!part) continue;
                        var current = PATH.join2(parent, part);
                        try {
                            FS.mkdir(current)
                        } catch (e) {
                        }
                        parent = current
                    }
                    return current
                },
                createFile: function (parent, name, properties, canRead, canWrite) {
                    var path = PATH.join2(typeof parent === "string" ? parent : FS.getPath(parent), name);
                    var mode = FS.getMode(canRead, canWrite);
                    return FS.create(path, mode)
                },
                createDataFile: function (parent, name, data, canRead, canWrite, canOwn) {
                    var path = name ? PATH.join2(typeof parent === "string" ? parent : FS.getPath(parent), name) : parent;
                    var mode = FS.getMode(canRead, canWrite);
                    var node = FS.create(path, mode);
                    if (data) {
                        if (typeof data === "string") {
                            var arr = new Array(data.length);
                            for (var i = 0, len = data.length; i < len; ++i) arr[i] = data.charCodeAt(i);
                            data = arr
                        }
                        FS.chmod(node, mode | 146);
                        var stream = FS.open(node, 577);
                        FS.write(stream, data, 0, data.length, 0, canOwn);
                        FS.close(stream);
                        FS.chmod(node, mode)
                    }
                    return node
                },
                createDevice: function (parent, name, input, output) {
                    var path = PATH.join2(typeof parent === "string" ? parent : FS.getPath(parent), name);
                    var mode = FS.getMode(!!input, !!output);
                    if (!FS.createDevice.major) FS.createDevice.major = 64;
                    var dev = FS.makedev(FS.createDevice.major++, 0);
                    FS.registerDevice(dev, {
                        open: function (stream) {
                            stream.seekable = false
                        }, close: function (stream) {
                            if (output && output.buffer && output.buffer.length) {
                                output(10)
                            }
                        }, read: function (stream, buffer, offset, length, pos) {
                            var bytesRead = 0;
                            for (var i = 0; i < length; i++) {
                                var result;
                                try {
                                    result = input()
                                } catch (e) {
                                    throw new FS.ErrnoError(29)
                                }
                                if (result === undefined && bytesRead === 0) {
                                    throw new FS.ErrnoError(6)
                                }
                                if (result === null || result === undefined) break;
                                bytesRead++;
                                buffer[offset + i] = result
                            }
                            if (bytesRead) {
                                stream.node.timestamp = Date.now()
                            }
                            return bytesRead
                        }, write: function (stream, buffer, offset, length, pos) {
                            for (var i = 0; i < length; i++) {
                                try {
                                    output(buffer[offset + i])
                                } catch (e) {
                                    throw new FS.ErrnoError(29)
                                }
                            }
                            if (length) {
                                stream.node.timestamp = Date.now()
                            }
                            return i
                        }
                    });
                    return FS.mkdev(path, mode, dev)
                },
                forceLoadFile: function (obj) {
                    if (obj.isDevice || obj.isFolder || obj.link || obj.contents) return true;
                    if (typeof XMLHttpRequest !== "undefined") {
                        throw new Error("Lazy loading should have been performed (contents set) in createLazyFile, but it was not. Lazy loading only works in web workers. Use --embed-file or --preload-file in emcc on the main thread.")
                    } else if (read_) {
                        try {
                            obj.contents = intArrayFromString(read_(obj.url), true);
                            obj.usedBytes = obj.contents.length
                        } catch (e) {
                            throw new FS.ErrnoError(29)
                        }
                    } else {
                        throw new Error("Cannot load without read() or XMLHttpRequest.")
                    }
                },
                createLazyFile: function (parent, name, url, canRead, canWrite) {
                    function LazyUint8Array() {
                        this.lengthKnown = false;
                        this.chunks = []
                    }

                    LazyUint8Array.prototype.get = function LazyUint8Array_get(idx) {
                        if (idx > this.length - 1 || idx < 0) {
                            return undefined
                        }
                        var chunkOffset = idx % this.chunkSize;
                        var chunkNum = idx / this.chunkSize | 0;
                        return this.getter(chunkNum)[chunkOffset]
                    };
                    LazyUint8Array.prototype.setDataGetter = function LazyUint8Array_setDataGetter(getter) {
                        this.getter = getter
                    };
                    LazyUint8Array.prototype.cacheLength = function LazyUint8Array_cacheLength() {
                        var xhr = new XMLHttpRequest;
                        xhr.open("HEAD", url, false);
                        xhr.send(null);
                        if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
                        var datalength = Number(xhr.getResponseHeader("Content-length"));
                        var header;
                        var hasByteServing = (header = xhr.getResponseHeader("Accept-Ranges")) && header === "bytes";
                        var usesGzip = (header = xhr.getResponseHeader("Content-Encoding")) && header === "gzip";
                        var chunkSize = 1024 * 1024;
                        if (!hasByteServing) chunkSize = datalength;
                        var doXHR = function (from, to) {
                            if (from > to) throw new Error("invalid range (" + from + ", " + to + ") or no bytes requested!");
                            if (to > datalength - 1) throw new Error("only " + datalength + " bytes available! programmer error!");
                            var xhr = new XMLHttpRequest;
                            xhr.open("GET", url, false);
                            if (datalength !== chunkSize) xhr.setRequestHeader("Range", "bytes=" + from + "-" + to);
                            if (typeof Uint8Array != "undefined") xhr.responseType = "arraybuffer";
                            if (xhr.overrideMimeType) {
                                xhr.overrideMimeType("text/plain; charset=x-user-defined")
                            }
                            xhr.send(null);
                            if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
                            if (xhr.response !== undefined) {
                                return new Uint8Array(xhr.response || [])
                            } else {
                                return intArrayFromString(xhr.responseText || "", true)
                            }
                        };
                        var lazyArray = this;
                        lazyArray.setDataGetter(function (chunkNum) {
                            var start = chunkNum * chunkSize;
                            var end = (chunkNum + 1) * chunkSize - 1;
                            end = Math.min(end, datalength - 1);
                            if (typeof lazyArray.chunks[chunkNum] === "undefined") {
                                lazyArray.chunks[chunkNum] = doXHR(start, end)
                            }
                            if (typeof lazyArray.chunks[chunkNum] === "undefined") throw new Error("doXHR failed!");
                            return lazyArray.chunks[chunkNum]
                        });
                        if (usesGzip || !datalength) {
                            chunkSize = datalength = 1;
                            datalength = this.getter(0).length;
                            chunkSize = datalength;
                            out("LazyFiles on gzip forces download of the whole file when length is accessed")
                        }
                        this._length = datalength;
                        this._chunkSize = chunkSize;
                        this.lengthKnown = true
                    };
                    if (typeof XMLHttpRequest !== "undefined") {
                        if (!ENVIRONMENT_IS_WORKER) throw"Cannot do synchronous binary XHRs outside webworkers in modern browsers. Use --embed-file or --preload-file in emcc";
                        var lazyArray = new LazyUint8Array;
                        Object.defineProperties(lazyArray, {
                            length: {
                                get: function () {
                                    if (!this.lengthKnown) {
                                        this.cacheLength()
                                    }
                                    return this._length
                                }
                            }, chunkSize: {
                                get: function () {
                                    if (!this.lengthKnown) {
                                        this.cacheLength()
                                    }
                                    return this._chunkSize
                                }
                            }
                        });
                        var properties = {isDevice: false, contents: lazyArray}
                    } else {
                        var properties = {isDevice: false, url: url}
                    }
                    var node = FS.createFile(parent, name, properties, canRead, canWrite);
                    if (properties.contents) {
                        node.contents = properties.contents
                    } else if (properties.url) {
                        node.contents = null;
                        node.url = properties.url
                    }
                    Object.defineProperties(node, {
                        usedBytes: {
                            get: function () {
                                return this.contents.length
                            }
                        }
                    });
                    var stream_ops = {};
                    var keys = Object.keys(node.stream_ops);
                    keys.forEach(function (key) {
                        var fn = node.stream_ops[key];
                        stream_ops[key] = function forceLoadLazyFile() {
                            FS.forceLoadFile(node);
                            return fn.apply(null, arguments)
                        }
                    });
                    stream_ops.read = function stream_ops_read(stream, buffer, offset, length, position) {
                        FS.forceLoadFile(node);
                        var contents = stream.node.contents;
                        if (position >= contents.length) return 0;
                        var size = Math.min(contents.length - position, length);
                        if (contents.slice) {
                            for (var i = 0; i < size; i++) {
                                buffer[offset + i] = contents[position + i]
                            }
                        } else {
                            for (var i = 0; i < size; i++) {
                                buffer[offset + i] = contents.get(position + i)
                            }
                        }
                        return size
                    };
                    node.stream_ops = stream_ops;
                    return node
                },
                createPreloadedFile: function (parent, name, url, canRead, canWrite, onload, onerror, dontCreateFile, canOwn, preFinish) {
                    Browser.init();
                    var fullname = name ? PATH_FS.resolve(PATH.join2(parent, name)) : parent;
                    var dep = getUniqueRunDependency("cp " + fullname);

                    function processData(byteArray) {
                        function finish(byteArray) {
                            if (preFinish) preFinish();
                            if (!dontCreateFile) {
                                FS.createDataFile(parent, name, byteArray, canRead, canWrite, canOwn)
                            }
                            if (onload) onload();
                            removeRunDependency(dep)
                        }

                        var handled = false;
                        Module["preloadPlugins"].forEach(function (plugin) {
                            if (handled) return;
                            if (plugin["canHandle"](fullname)) {
                                plugin["handle"](byteArray, fullname, finish, function () {
                                    if (onerror) onerror();
                                    removeRunDependency(dep)
                                });
                                handled = true
                            }
                        });
                        if (!handled) finish(byteArray)
                    }

                    addRunDependency(dep);
                    if (typeof url == "string") {
                        asyncLoad(url, function (byteArray) {
                            processData(byteArray)
                        }, onerror)
                    } else {
                        processData(url)
                    }
                },
                indexedDB: function () {
                    return window.indexedDB || window.mozIndexedDB || window.webkitIndexedDB || window.msIndexedDB
                },
                DB_NAME: function () {
                    return "EM_FS_" + window.location.pathname
                },
                DB_VERSION: 20,
                DB_STORE_NAME: "FILE_DATA",
                saveFilesToDB: function (paths, onload, onerror) {
                    onload = onload || function () {
                    };
                    onerror = onerror || function () {
                    };
                    var indexedDB = FS.indexedDB();
                    try {
                        var openRequest = indexedDB.open(FS.DB_NAME(), FS.DB_VERSION)
                    } catch (e) {
                        return onerror(e)
                    }
                    openRequest.onupgradeneeded = function openRequest_onupgradeneeded() {
                        out("creating db");
                        var db = openRequest.result;
                        db.createObjectStore(FS.DB_STORE_NAME)
                    };
                    openRequest.onsuccess = function openRequest_onsuccess() {
                        var db = openRequest.result;
                        var transaction = db.transaction([FS.DB_STORE_NAME], "readwrite");
                        var files = transaction.objectStore(FS.DB_STORE_NAME);
                        var ok = 0, fail = 0, total = paths.length;

                        function finish() {
                            if (fail == 0) onload(); else onerror()
                        }

                        paths.forEach(function (path) {
                            var putRequest = files.put(FS.analyzePath(path).object.contents, path);
                            putRequest.onsuccess = function putRequest_onsuccess() {
                                ok++;
                                if (ok + fail == total) finish()
                            };
                            putRequest.onerror = function putRequest_onerror() {
                                fail++;
                                if (ok + fail == total) finish()
                            }
                        });
                        transaction.onerror = onerror
                    };
                    openRequest.onerror = onerror
                },
                loadFilesFromDB: function (paths, onload, onerror) {
                    onload = onload || function () {
                    };
                    onerror = onerror || function () {
                    };
                    var indexedDB = FS.indexedDB();
                    try {
                        var openRequest = indexedDB.open(FS.DB_NAME(), FS.DB_VERSION)
                    } catch (e) {
                        return onerror(e)
                    }
                    openRequest.onupgradeneeded = onerror;
                    openRequest.onsuccess = function openRequest_onsuccess() {
                        var db = openRequest.result;
                        try {
                            var transaction = db.transaction([FS.DB_STORE_NAME], "readonly")
                        } catch (e) {
                            onerror(e);
                            return
                        }
                        var files = transaction.objectStore(FS.DB_STORE_NAME);
                        var ok = 0, fail = 0, total = paths.length;

                        function finish() {
                            if (fail == 0) onload(); else onerror()
                        }

                        paths.forEach(function (path) {
                            var getRequest = files.get(path);
                            getRequest.onsuccess = function getRequest_onsuccess() {
                                if (FS.analyzePath(path).exists) {
                                    FS.unlink(path)
                                }
                                FS.createDataFile(PATH.dirname(path), PATH.basename(path), getRequest.result, true, true, true);
                                ok++;
                                if (ok + fail == total) finish()
                            };
                            getRequest.onerror = function getRequest_onerror() {
                                fail++;
                                if (ok + fail == total) finish()
                            }
                        });
                        transaction.onerror = onerror
                    };
                    openRequest.onerror = onerror
                }
            };
            var SYSCALLS = {
                mappings: {}, DEFAULT_POLLMASK: 5, umask: 511, calculateAt: function (dirfd, path, allowEmpty) {
                    if (path[0] === "/") {
                        return path
                    }
                    var dir;
                    if (dirfd === -100) {
                        dir = FS.cwd()
                    } else {
                        var dirstream = FS.getStream(dirfd);
                        if (!dirstream) throw new FS.ErrnoError(8);
                        dir = dirstream.path
                    }
                    if (path.length == 0) {
                        if (!allowEmpty) {
                            throw new FS.ErrnoError(44)
                        }
                        return dir
                    }
                    return PATH.join2(dir, path)
                }, doStat: function (func, path, buf) {
                    try {
                        var stat = func(path)
                    } catch (e) {
                        if (e && e.node && PATH.normalize(path) !== PATH.normalize(FS.getPath(e.node))) {
                            return -54
                        }
                        throw e
                    }
                    HEAP32[buf >> 2] = stat.dev;
                    HEAP32[buf + 4 >> 2] = 0;
                    HEAP32[buf + 8 >> 2] = stat.ino;
                    HEAP32[buf + 12 >> 2] = stat.mode;
                    HEAP32[buf + 16 >> 2] = stat.nlink;
                    HEAP32[buf + 20 >> 2] = stat.uid;
                    HEAP32[buf + 24 >> 2] = stat.gid;
                    HEAP32[buf + 28 >> 2] = stat.rdev;
                    HEAP32[buf + 32 >> 2] = 0;
                    tempI64 = [stat.size >>> 0, (tempDouble = stat.size, +Math.abs(tempDouble) >= 1 ? tempDouble > 0 ? (Math.min(+Math.floor(tempDouble / 4294967296), 4294967295) | 0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296) >>> 0 : 0)], HEAP32[buf + 40 >> 2] = tempI64[0], HEAP32[buf + 44 >> 2] = tempI64[1];
                    HEAP32[buf + 48 >> 2] = 4096;
                    HEAP32[buf + 52 >> 2] = stat.blocks;
                    HEAP32[buf + 56 >> 2] = stat.atime.getTime() / 1e3 | 0;
                    HEAP32[buf + 60 >> 2] = 0;
                    HEAP32[buf + 64 >> 2] = stat.mtime.getTime() / 1e3 | 0;
                    HEAP32[buf + 68 >> 2] = 0;
                    HEAP32[buf + 72 >> 2] = stat.ctime.getTime() / 1e3 | 0;
                    HEAP32[buf + 76 >> 2] = 0;
                    tempI64 = [stat.ino >>> 0, (tempDouble = stat.ino, +Math.abs(tempDouble) >= 1 ? tempDouble > 0 ? (Math.min(+Math.floor(tempDouble / 4294967296), 4294967295) | 0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296) >>> 0 : 0)], HEAP32[buf + 80 >> 2] = tempI64[0], HEAP32[buf + 84 >> 2] = tempI64[1];
                    return 0
                }, doMsync: function (addr, stream, len, flags, offset) {
                    var buffer = HEAPU8.slice(addr, addr + len);
                    FS.msync(stream, buffer, offset, len, flags)
                }, doMkdir: function (path, mode) {
                    path = PATH.normalize(path);
                    if (path[path.length - 1] === "/") path = path.substr(0, path.length - 1);
                    FS.mkdir(path, mode, 0);
                    return 0
                }, doMknod: function (path, mode, dev) {
                    switch (mode & 61440) {
                        case 32768:
                        case 8192:
                        case 24576:
                        case 4096:
                        case 49152:
                            break;
                        default:
                            return -28
                    }
                    FS.mknod(path, mode, dev);
                    return 0
                }, doReadlink: function (path, buf, bufsize) {
                    if (bufsize <= 0) return -28;
                    var ret = FS.readlink(path);
                    var len = Math.min(bufsize, lengthBytesUTF8(ret));
                    var endChar = HEAP8[buf + len];
                    stringToUTF8(ret, buf, bufsize + 1);
                    HEAP8[buf + len] = endChar;
                    return len
                }, doAccess: function (path, amode) {
                    if (amode & ~7) {
                        return -28
                    }
                    var node;
                    var lookup = FS.lookupPath(path, {follow: true});
                    node = lookup.node;
                    if (!node) {
                        return -44
                    }
                    var perms = "";
                    if (amode & 4) perms += "r";
                    if (amode & 2) perms += "w";
                    if (amode & 1) perms += "x";
                    if (perms && FS.nodePermissions(node, perms)) {
                        return -2
                    }
                    return 0
                }, doDup: function (path, flags, suggestFD) {
                    var suggest = FS.getStream(suggestFD);
                    if (suggest) FS.close(suggest);
                    return FS.open(path, flags, 0, suggestFD, suggestFD).fd
                }, doReadv: function (stream, iov, iovcnt, offset) {
                    var ret = 0;
                    for (var i = 0; i < iovcnt; i++) {
                        var ptr = HEAP32[iov + i * 8 >> 2];
                        var len = HEAP32[iov + (i * 8 + 4) >> 2];
                        var curr = FS.read(stream, HEAP8, ptr, len, offset);
                        if (curr < 0) return -1;
                        ret += curr;
                        if (curr < len) break
                    }
                    return ret
                }, doWritev: function (stream, iov, iovcnt, offset) {
                    var ret = 0;
                    for (var i = 0; i < iovcnt; i++) {
                        var ptr = HEAP32[iov + i * 8 >> 2];
                        var len = HEAP32[iov + (i * 8 + 4) >> 2];
                        var curr = FS.write(stream, HEAP8, ptr, len, offset);
                        if (curr < 0) return -1;
                        ret += curr
                    }
                    return ret
                }, varargs: undefined, get: function () {
                    SYSCALLS.varargs += 4;
                    var ret = HEAP32[SYSCALLS.varargs - 4 >> 2];
                    return ret
                }, getStr: function (ptr) {
                    var ret = UTF8ToString(ptr);
                    return ret
                }, getStreamFromFD: function (fd) {
                    var stream = FS.getStream(fd);
                    if (!stream) throw new FS.ErrnoError(8);
                    return stream
                }, get64: function (low, high) {
                    return low
                }
            };

            function ___sys_fcntl64(fd, cmd, varargs) {
                SYSCALLS.varargs = varargs;
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    switch (cmd) {
                        case 0: {
                            var arg = SYSCALLS.get();
                            if (arg < 0) {
                                return -28
                            }
                            var newStream;
                            newStream = FS.open(stream.path, stream.flags, 0, arg);
                            return newStream.fd
                        }
                        case 1:
                        case 2:
                            return 0;
                        case 3:
                            return stream.flags;
                        case 4: {
                            var arg = SYSCALLS.get();
                            stream.flags |= arg;
                            return 0
                        }
                        case 12: {
                            var arg = SYSCALLS.get();
                            var offset = 0;
                            HEAP16[arg + offset >> 1] = 2;
                            return 0
                        }
                        case 13:
                        case 14:
                            return 0;
                        case 16:
                        case 8:
                            return -28;
                        case 9:
                            setErrNo(28);
                            return -1;
                        default: {
                            return -28
                        }
                    }
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return -e.errno
                }
            }

            function ___sys_getdents64(fd, dirp, count) {
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    if (!stream.getdents) {
                        stream.getdents = FS.readdir(stream.path)
                    }
                    var struct_size = 280;
                    var pos = 0;
                    var off = FS.llseek(stream, 0, 1);
                    var idx = Math.floor(off / struct_size);
                    while (idx < stream.getdents.length && pos + struct_size <= count) {
                        var id;
                        var type;
                        var name = stream.getdents[idx];
                        if (name[0] === ".") {
                            id = 1;
                            type = 4
                        } else {
                            var child = FS.lookupNode(stream.node, name);
                            id = child.id;
                            type = FS.isChrdev(child.mode) ? 2 : FS.isDir(child.mode) ? 4 : FS.isLink(child.mode) ? 10 : 8
                        }
                        tempI64 = [id >>> 0, (tempDouble = id, +Math.abs(tempDouble) >= 1 ? tempDouble > 0 ? (Math.min(+Math.floor(tempDouble / 4294967296), 4294967295) | 0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296) >>> 0 : 0)], HEAP32[dirp + pos >> 2] = tempI64[0], HEAP32[dirp + pos + 4 >> 2] = tempI64[1];
                        tempI64 = [(idx + 1) * struct_size >>> 0, (tempDouble = (idx + 1) * struct_size, +Math.abs(tempDouble) >= 1 ? tempDouble > 0 ? (Math.min(+Math.floor(tempDouble / 4294967296), 4294967295) | 0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296) >>> 0 : 0)], HEAP32[dirp + pos + 8 >> 2] = tempI64[0], HEAP32[dirp + pos + 12 >> 2] = tempI64[1];
                        HEAP16[dirp + pos + 16 >> 1] = 280;
                        HEAP8[dirp + pos + 18 >> 0] = type;
                        stringToUTF8(name, dirp + pos + 19, 256);
                        pos += struct_size;
                        idx += 1
                    }
                    FS.llseek(stream, idx * struct_size, 0);
                    return pos
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return -e.errno
                }
            }

            function ___sys_ioctl(fd, op, varargs) {
                SYSCALLS.varargs = varargs;
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    switch (op) {
                        case 21509:
                        case 21505: {
                            if (!stream.tty) return -59;
                            return 0
                        }
                        case 21510:
                        case 21511:
                        case 21512:
                        case 21506:
                        case 21507:
                        case 21508: {
                            if (!stream.tty) return -59;
                            return 0
                        }
                        case 21519: {
                            if (!stream.tty) return -59;
                            var argp = SYSCALLS.get();
                            HEAP32[argp >> 2] = 0;
                            return 0
                        }
                        case 21520: {
                            if (!stream.tty) return -59;
                            return -28
                        }
                        case 21531: {
                            var argp = SYSCALLS.get();
                            return FS.ioctl(stream, op, argp)
                        }
                        case 21523: {
                            if (!stream.tty) return -59;
                            return 0
                        }
                        case 21524: {
                            if (!stream.tty) return -59;
                            return 0
                        }
                        default:
                            abort("bad ioctl syscall " + op)
                    }
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return -e.errno
                }
            }

            function ___sys_open(path, flags, varargs) {
                SYSCALLS.varargs = varargs;
                try {
                    var pathname = SYSCALLS.getStr(path);
                    var mode = varargs ? SYSCALLS.get() : 0;
                    var stream = FS.open(pathname, flags, mode);
                    return stream.fd
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return -e.errno
                }
            }

            function ___sys_stat64(path, buf) {
                try {
                    path = SYSCALLS.getStr(path);
                    return SYSCALLS.doStat(FS.stat, path, buf)
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return -e.errno
                }
            }

            var tupleRegistrations = {};

            function runDestructors(destructors) {
                while (destructors.length) {
                    var ptr = destructors.pop();
                    var del = destructors.pop();
                    del(ptr)
                }
            }

            function simpleReadValueFromPointer(pointer) {
                return this["fromWireType"](HEAPU32[pointer >> 2])
            }

            var awaitingDependencies = {};
            var registeredTypes = {};
            var typeDependencies = {};
            var char_0 = 48;
            var char_9 = 57;

            function makeLegalFunctionName(name) {
                if (undefined === name) {
                    return "_unknown"
                }
                name = name.replace(/[^a-zA-Z0-9_]/g, "$");
                var f = name.charCodeAt(0);
                if (f >= char_0 && f <= char_9) {
                    return "_" + name
                } else {
                    return name
                }
            }

            function createNamedFunction(name, body) {
                name = makeLegalFunctionName(name);
                return new Function("body", "return function " + name + "() {\n" + '    "use strict";' + "    return body.apply(this, arguments);\n" + "};\n")(body)
            }

            function extendError(baseErrorType, errorName) {
                var errorClass = createNamedFunction(errorName, function (message) {
                    this.name = errorName;
                    this.message = message;
                    var stack = new Error(message).stack;
                    if (stack !== undefined) {
                        this.stack = this.toString() + "\n" + stack.replace(/^Error(:[^\n]*)?\n/, "")
                    }
                });
                errorClass.prototype = Object.create(baseErrorType.prototype);
                errorClass.prototype.constructor = errorClass;
                errorClass.prototype.toString = function () {
                    if (this.message === undefined) {
                        return this.name
                    } else {
                        return this.name + ": " + this.message
                    }
                };
                return errorClass
            }

            var InternalError = undefined;

            function throwInternalError(message) {
                throw new InternalError(message)
            }

            function whenDependentTypesAreResolved(myTypes, dependentTypes, getTypeConverters) {
                myTypes.forEach(function (type) {
                    typeDependencies[type] = dependentTypes
                });

                function onComplete(typeConverters) {
                    var myTypeConverters = getTypeConverters(typeConverters);
                    if (myTypeConverters.length !== myTypes.length) {
                        throwInternalError("Mismatched type converter count")
                    }
                    for (var i = 0; i < myTypes.length; ++i) {
                        registerType(myTypes[i], myTypeConverters[i])
                    }
                }

                var typeConverters = new Array(dependentTypes.length);
                var unregisteredTypes = [];
                var registered = 0;
                dependentTypes.forEach(function (dt, i) {
                    if (registeredTypes.hasOwnProperty(dt)) {
                        typeConverters[i] = registeredTypes[dt]
                    } else {
                        unregisteredTypes.push(dt);
                        if (!awaitingDependencies.hasOwnProperty(dt)) {
                            awaitingDependencies[dt] = []
                        }
                        awaitingDependencies[dt].push(function () {
                            typeConverters[i] = registeredTypes[dt];
                            ++registered;
                            if (registered === unregisteredTypes.length) {
                                onComplete(typeConverters)
                            }
                        })
                    }
                });
                if (0 === unregisteredTypes.length) {
                    onComplete(typeConverters)
                }
            }

            function __embind_finalize_value_array(rawTupleType) {
                var reg = tupleRegistrations[rawTupleType];
                delete tupleRegistrations[rawTupleType];
                var elements = reg.elements;
                var elementsLength = elements.length;
                var elementTypes = elements.map(function (elt) {
                    return elt.getterReturnType
                }).concat(elements.map(function (elt) {
                    return elt.setterArgumentType
                }));
                var rawConstructor = reg.rawConstructor;
                var rawDestructor = reg.rawDestructor;
                whenDependentTypesAreResolved([rawTupleType], elementTypes, function (elementTypes) {
                    elements.forEach(function (elt, i) {
                        var getterReturnType = elementTypes[i];
                        var getter = elt.getter;
                        var getterContext = elt.getterContext;
                        var setterArgumentType = elementTypes[i + elementsLength];
                        var setter = elt.setter;
                        var setterContext = elt.setterContext;
                        elt.read = function (ptr) {
                            return getterReturnType["fromWireType"](getter(getterContext, ptr))
                        };
                        elt.write = function (ptr, o) {
                            var destructors = [];
                            setter(setterContext, ptr, setterArgumentType["toWireType"](destructors, o));
                            runDestructors(destructors)
                        }
                    });
                    return [{
                        name: reg.name,
                        "fromWireType": function (ptr) {
                            var rv = new Array(elementsLength);
                            for (var i = 0; i < elementsLength; ++i) {
                                rv[i] = elements[i].read(ptr)
                            }
                            rawDestructor(ptr);
                            return rv
                        },
                        "toWireType": function (destructors, o) {
                            if (elementsLength !== o.length) {
                                throw new TypeError("Incorrect number of tuple elements for " + reg.name + ": expected=" + elementsLength + ", actual=" + o.length)
                            }
                            var ptr = rawConstructor();
                            for (var i = 0; i < elementsLength; ++i) {
                                elements[i].write(ptr, o[i])
                            }
                            if (destructors !== null) {
                                destructors.push(rawDestructor, ptr)
                            }
                            return ptr
                        },
                        "argPackAdvance": 8,
                        "readValueFromPointer": simpleReadValueFromPointer,
                        destructorFunction: rawDestructor
                    }]
                })
            }

            function __embind_register_bigint(primitiveType, name, size, minRange, maxRange) {
            }

            function getShiftFromSize(size) {
                switch (size) {
                    case 1:
                        return 0;
                    case 2:
                        return 1;
                    case 4:
                        return 2;
                    case 8:
                        return 3;
                    default:
                        throw new TypeError("Unknown type size: " + size)
                }
            }

            function embind_init_charCodes() {
                var codes = new Array(256);
                for (var i = 0; i < 256; ++i) {
                    codes[i] = String.fromCharCode(i)
                }
                embind_charCodes = codes
            }

            var embind_charCodes = undefined;

            function readLatin1String(ptr) {
                var ret = "";
                var c = ptr;
                while (HEAPU8[c]) {
                    ret += embind_charCodes[HEAPU8[c++]]
                }
                return ret
            }

            var BindingError = undefined;

            function throwBindingError(message) {
                throw new BindingError(message)
            }

            function registerType(rawType, registeredInstance, options) {
                options = options || {};
                if (!("argPackAdvance" in registeredInstance)) {
                    throw new TypeError("registerType registeredInstance requires argPackAdvance")
                }
                var name = registeredInstance.name;
                if (!rawType) {
                    throwBindingError('type "' + name + '" must have a positive integer typeid pointer')
                }
                if (registeredTypes.hasOwnProperty(rawType)) {
                    if (options.ignoreDuplicateRegistrations) {
                        return
                    } else {
                        throwBindingError("Cannot register type '" + name + "' twice")
                    }
                }
                registeredTypes[rawType] = registeredInstance;
                delete typeDependencies[rawType];
                if (awaitingDependencies.hasOwnProperty(rawType)) {
                    var callbacks = awaitingDependencies[rawType];
                    delete awaitingDependencies[rawType];
                    callbacks.forEach(function (cb) {
                        cb()
                    })
                }
            }

            function __embind_register_bool(rawType, name, size, trueValue, falseValue) {
                var shift = getShiftFromSize(size);
                name = readLatin1String(name);
                registerType(rawType, {
                    name: name, "fromWireType": function (wt) {
                        return !!wt
                    }, "toWireType": function (destructors, o) {
                        return o ? trueValue : falseValue
                    }, "argPackAdvance": 8, "readValueFromPointer": function (pointer) {
                        var heap;
                        if (size === 1) {
                            heap = HEAP8
                        } else if (size === 2) {
                            heap = HEAP16
                        } else if (size === 4) {
                            heap = HEAP32
                        } else {
                            throw new TypeError("Unknown boolean type size: " + name)
                        }
                        return this["fromWireType"](heap[pointer >> shift])
                    }, destructorFunction: null
                })
            }

            function ClassHandle_isAliasOf(other) {
                if (!(this instanceof ClassHandle)) {
                    return false
                }
                if (!(other instanceof ClassHandle)) {
                    return false
                }
                var leftClass = this.$$.ptrType.registeredClass;
                var left = this.$$.ptr;
                var rightClass = other.$$.ptrType.registeredClass;
                var right = other.$$.ptr;
                while (leftClass.baseClass) {
                    left = leftClass.upcast(left);
                    leftClass = leftClass.baseClass
                }
                while (rightClass.baseClass) {
                    right = rightClass.upcast(right);
                    rightClass = rightClass.baseClass
                }
                return leftClass === rightClass && left === right
            }

            function shallowCopyInternalPointer(o) {
                return {
                    count: o.count,
                    deleteScheduled: o.deleteScheduled,
                    preservePointerOnDelete: o.preservePointerOnDelete,
                    ptr: o.ptr,
                    ptrType: o.ptrType,
                    smartPtr: o.smartPtr,
                    smartPtrType: o.smartPtrType
                }
            }

            function throwInstanceAlreadyDeleted(obj) {
                function getInstanceTypeName(handle) {
                    return handle.$$.ptrType.registeredClass.name
                }

                throwBindingError(getInstanceTypeName(obj) + " instance already deleted")
            }

            var finalizationGroup = false;

            function detachFinalizer(handle) {
            }

            function runDestructor($$) {
                if ($$.smartPtr) {
                    $$.smartPtrType.rawDestructor($$.smartPtr)
                } else {
                    $$.ptrType.registeredClass.rawDestructor($$.ptr)
                }
            }

            function releaseClassHandle($$) {
                $$.count.value -= 1;
                var toDelete = 0 === $$.count.value;
                if (toDelete) {
                    runDestructor($$)
                }
            }

            function attachFinalizer(handle) {
                if ("undefined" === typeof FinalizationGroup) {
                    attachFinalizer = function (handle) {
                        return handle
                    };
                    return handle
                }
                finalizationGroup = new FinalizationGroup(function (iter) {
                    for (var result = iter.next(); !result.done; result = iter.next()) {
                        var $$ = result.value;
                        if (!$$.ptr) {
                            console.warn("object already deleted: " + $$.ptr)
                        } else {
                            releaseClassHandle($$)
                        }
                    }
                });
                attachFinalizer = function (handle) {
                    finalizationGroup.register(handle, handle.$$, handle.$$);
                    return handle
                };
                detachFinalizer = function (handle) {
                    finalizationGroup.unregister(handle.$$)
                };
                return attachFinalizer(handle)
            }

            function ClassHandle_clone() {
                if (!this.$$.ptr) {
                    throwInstanceAlreadyDeleted(this)
                }
                if (this.$$.preservePointerOnDelete) {
                    this.$$.count.value += 1;
                    return this
                } else {
                    var clone = attachFinalizer(Object.create(Object.getPrototypeOf(this), {$$: {value: shallowCopyInternalPointer(this.$$)}}));
                    clone.$$.count.value += 1;
                    clone.$$.deleteScheduled = false;
                    return clone
                }
            }

            function ClassHandle_delete() {
                if (!this.$$.ptr) {
                    throwInstanceAlreadyDeleted(this)
                }
                if (this.$$.deleteScheduled && !this.$$.preservePointerOnDelete) {
                    throwBindingError("Object already scheduled for deletion")
                }
                detachFinalizer(this);
                releaseClassHandle(this.$$);
                if (!this.$$.preservePointerOnDelete) {
                    this.$$.smartPtr = undefined;
                    this.$$.ptr = undefined
                }
            }

            function ClassHandle_isDeleted() {
                return !this.$$.ptr
            }

            var delayFunction = undefined;
            var deletionQueue = [];

            function flushPendingDeletes() {
                while (deletionQueue.length) {
                    var obj = deletionQueue.pop();
                    obj.$$.deleteScheduled = false;
                    obj["delete"]()
                }
            }

            function ClassHandle_deleteLater() {
                if (!this.$$.ptr) {
                    throwInstanceAlreadyDeleted(this)
                }
                if (this.$$.deleteScheduled && !this.$$.preservePointerOnDelete) {
                    throwBindingError("Object already scheduled for deletion")
                }
                deletionQueue.push(this);
                if (deletionQueue.length === 1 && delayFunction) {
                    delayFunction(flushPendingDeletes)
                }
                this.$$.deleteScheduled = true;
                return this
            }

            function init_ClassHandle() {
                ClassHandle.prototype["isAliasOf"] = ClassHandle_isAliasOf;
                ClassHandle.prototype["clone"] = ClassHandle_clone;
                ClassHandle.prototype["delete"] = ClassHandle_delete;
                ClassHandle.prototype["isDeleted"] = ClassHandle_isDeleted;
                ClassHandle.prototype["deleteLater"] = ClassHandle_deleteLater
            }

            function ClassHandle() {
            }

            var registeredPointers = {};

            function ensureOverloadTable(proto, methodName, humanName) {
                if (undefined === proto[methodName].overloadTable) {
                    var prevFunc = proto[methodName];
                    proto[methodName] = function () {
                        if (!proto[methodName].overloadTable.hasOwnProperty(arguments.length)) {
                            throwBindingError("Function '" + humanName + "' called with an invalid number of arguments (" + arguments.length + ") - expects one of (" + proto[methodName].overloadTable + ")!")
                        }
                        return proto[methodName].overloadTable[arguments.length].apply(this, arguments)
                    };
                    proto[methodName].overloadTable = [];
                    proto[methodName].overloadTable[prevFunc.argCount] = prevFunc
                }
            }

            function exposePublicSymbol(name, value, numArguments) {
                if (Module.hasOwnProperty(name)) {
                    if (undefined === numArguments || undefined !== Module[name].overloadTable && undefined !== Module[name].overloadTable[numArguments]) {
                        throwBindingError("Cannot register public name '" + name + "' twice")
                    }
                    ensureOverloadTable(Module, name, name);
                    if (Module.hasOwnProperty(numArguments)) {
                        throwBindingError("Cannot register multiple overloads of a function with the same number of arguments (" + numArguments + ")!")
                    }
                    Module[name].overloadTable[numArguments] = value
                } else {
                    Module[name] = value;
                    if (undefined !== numArguments) {
                        Module[name].numArguments = numArguments
                    }
                }
            }

            function RegisteredClass(name, constructor, instancePrototype, rawDestructor, baseClass, getActualType, upcast, downcast) {
                this.name = name;
                this.constructor = constructor;
                this.instancePrototype = instancePrototype;
                this.rawDestructor = rawDestructor;
                this.baseClass = baseClass;
                this.getActualType = getActualType;
                this.upcast = upcast;
                this.downcast = downcast;
                this.pureVirtualFunctions = []
            }

            function upcastPointer(ptr, ptrClass, desiredClass) {
                while (ptrClass !== desiredClass) {
                    if (!ptrClass.upcast) {
                        throwBindingError("Expected null or instance of " + desiredClass.name + ", got an instance of " + ptrClass.name)
                    }
                    ptr = ptrClass.upcast(ptr);
                    ptrClass = ptrClass.baseClass
                }
                return ptr
            }

            function constNoSmartPtrRawPointerToWireType(destructors, handle) {
                if (handle === null) {
                    if (this.isReference) {
                        throwBindingError("null is not a valid " + this.name)
                    }
                    return 0
                }
                if (!handle.$$) {
                    throwBindingError('Cannot pass "' + _embind_repr(handle) + '" as a ' + this.name)
                }
                if (!handle.$$.ptr) {
                    throwBindingError("Cannot pass deleted object as a pointer of type " + this.name)
                }
                var handleClass = handle.$$.ptrType.registeredClass;
                var ptr = upcastPointer(handle.$$.ptr, handleClass, this.registeredClass);
                return ptr
            }

            function genericPointerToWireType(destructors, handle) {
                var ptr;
                if (handle === null) {
                    if (this.isReference) {
                        throwBindingError("null is not a valid " + this.name)
                    }
                    if (this.isSmartPointer) {
                        ptr = this.rawConstructor();
                        if (destructors !== null) {
                            destructors.push(this.rawDestructor, ptr)
                        }
                        return ptr
                    } else {
                        return 0
                    }
                }
                if (!handle.$$) {
                    throwBindingError('Cannot pass "' + _embind_repr(handle) + '" as a ' + this.name)
                }
                if (!handle.$$.ptr) {
                    throwBindingError("Cannot pass deleted object as a pointer of type " + this.name)
                }
                if (!this.isConst && handle.$$.ptrType.isConst) {
                    throwBindingError("Cannot convert argument of type " + (handle.$$.smartPtrType ? handle.$$.smartPtrType.name : handle.$$.ptrType.name) + " to parameter type " + this.name)
                }
                var handleClass = handle.$$.ptrType.registeredClass;
                ptr = upcastPointer(handle.$$.ptr, handleClass, this.registeredClass);
                if (this.isSmartPointer) {
                    if (undefined === handle.$$.smartPtr) {
                        throwBindingError("Passing raw pointer to smart pointer is illegal")
                    }
                    switch (this.sharingPolicy) {
                        case 0:
                            if (handle.$$.smartPtrType === this) {
                                ptr = handle.$$.smartPtr
                            } else {
                                throwBindingError("Cannot convert argument of type " + (handle.$$.smartPtrType ? handle.$$.smartPtrType.name : handle.$$.ptrType.name) + " to parameter type " + this.name)
                            }
                            break;
                        case 1:
                            ptr = handle.$$.smartPtr;
                            break;
                        case 2:
                            if (handle.$$.smartPtrType === this) {
                                ptr = handle.$$.smartPtr
                            } else {
                                var clonedHandle = handle["clone"]();
                                ptr = this.rawShare(ptr, __emval_register(function () {
                                    clonedHandle["delete"]()
                                }));
                                if (destructors !== null) {
                                    destructors.push(this.rawDestructor, ptr)
                                }
                            }
                            break;
                        default:
                            throwBindingError("Unsupporting sharing policy")
                    }
                }
                return ptr
            }

            function nonConstNoSmartPtrRawPointerToWireType(destructors, handle) {
                if (handle === null) {
                    if (this.isReference) {
                        throwBindingError("null is not a valid " + this.name)
                    }
                    return 0
                }
                if (!handle.$$) {
                    throwBindingError('Cannot pass "' + _embind_repr(handle) + '" as a ' + this.name)
                }
                if (!handle.$$.ptr) {
                    throwBindingError("Cannot pass deleted object as a pointer of type " + this.name)
                }
                if (handle.$$.ptrType.isConst) {
                    throwBindingError("Cannot convert argument of type " + handle.$$.ptrType.name + " to parameter type " + this.name)
                }
                var handleClass = handle.$$.ptrType.registeredClass;
                var ptr = upcastPointer(handle.$$.ptr, handleClass, this.registeredClass);
                return ptr
            }

            function RegisteredPointer_getPointee(ptr) {
                if (this.rawGetPointee) {
                    ptr = this.rawGetPointee(ptr)
                }
                return ptr
            }

            function RegisteredPointer_destructor(ptr) {
                if (this.rawDestructor) {
                    this.rawDestructor(ptr)
                }
            }

            function RegisteredPointer_deleteObject(handle) {
                if (handle !== null) {
                    handle["delete"]()
                }
            }

            function downcastPointer(ptr, ptrClass, desiredClass) {
                if (ptrClass === desiredClass) {
                    return ptr
                }
                if (undefined === desiredClass.baseClass) {
                    return null
                }
                var rv = downcastPointer(ptr, ptrClass, desiredClass.baseClass);
                if (rv === null) {
                    return null
                }
                return desiredClass.downcast(rv)
            }

            function getInheritedInstanceCount() {
                return Object.keys(registeredInstances).length
            }

            function getLiveInheritedInstances() {
                var rv = [];
                for (var k in registeredInstances) {
                    if (registeredInstances.hasOwnProperty(k)) {
                        rv.push(registeredInstances[k])
                    }
                }
                return rv
            }

            function setDelayFunction(fn) {
                delayFunction = fn;
                if (deletionQueue.length && delayFunction) {
                    delayFunction(flushPendingDeletes)
                }
            }

            function init_embind() {
                Module["getInheritedInstanceCount"] = getInheritedInstanceCount;
                Module["getLiveInheritedInstances"] = getLiveInheritedInstances;
                Module["flushPendingDeletes"] = flushPendingDeletes;
                Module["setDelayFunction"] = setDelayFunction
            }

            var registeredInstances = {};

            function getBasestPointer(class_, ptr) {
                if (ptr === undefined) {
                    throwBindingError("ptr should not be undefined")
                }
                while (class_.baseClass) {
                    ptr = class_.upcast(ptr);
                    class_ = class_.baseClass
                }
                return ptr
            }

            function getInheritedInstance(class_, ptr) {
                ptr = getBasestPointer(class_, ptr);
                return registeredInstances[ptr]
            }

            function makeClassHandle(prototype, record) {
                if (!record.ptrType || !record.ptr) {
                    throwInternalError("makeClassHandle requires ptr and ptrType")
                }
                var hasSmartPtrType = !!record.smartPtrType;
                var hasSmartPtr = !!record.smartPtr;
                if (hasSmartPtrType !== hasSmartPtr) {
                    throwInternalError("Both smartPtrType and smartPtr must be specified")
                }
                record.count = {value: 1};
                return attachFinalizer(Object.create(prototype, {$$: {value: record}}))
            }

            function RegisteredPointer_fromWireType(ptr) {
                var rawPointer = this.getPointee(ptr);
                if (!rawPointer) {
                    this.destructor(ptr);
                    return null
                }
                var registeredInstance = getInheritedInstance(this.registeredClass, rawPointer);
                if (undefined !== registeredInstance) {
                    if (0 === registeredInstance.$$.count.value) {
                        registeredInstance.$$.ptr = rawPointer;
                        registeredInstance.$$.smartPtr = ptr;
                        return registeredInstance["clone"]()
                    } else {
                        var rv = registeredInstance["clone"]();
                        this.destructor(ptr);
                        return rv
                    }
                }

                function makeDefaultHandle() {
                    if (this.isSmartPointer) {
                        return makeClassHandle(this.registeredClass.instancePrototype, {
                            ptrType: this.pointeeType,
                            ptr: rawPointer,
                            smartPtrType: this,
                            smartPtr: ptr
                        })
                    } else {
                        return makeClassHandle(this.registeredClass.instancePrototype, {ptrType: this, ptr: ptr})
                    }
                }

                var actualType = this.registeredClass.getActualType(rawPointer);
                var registeredPointerRecord = registeredPointers[actualType];
                if (!registeredPointerRecord) {
                    return makeDefaultHandle.call(this)
                }
                var toType;
                if (this.isConst) {
                    toType = registeredPointerRecord.constPointerType
                } else {
                    toType = registeredPointerRecord.pointerType
                }
                var dp = downcastPointer(rawPointer, this.registeredClass, toType.registeredClass);
                if (dp === null) {
                    return makeDefaultHandle.call(this)
                }
                if (this.isSmartPointer) {
                    return makeClassHandle(toType.registeredClass.instancePrototype, {
                        ptrType: toType,
                        ptr: dp,
                        smartPtrType: this,
                        smartPtr: ptr
                    })
                } else {
                    return makeClassHandle(toType.registeredClass.instancePrototype, {ptrType: toType, ptr: dp})
                }
            }

            function init_RegisteredPointer() {
                RegisteredPointer.prototype.getPointee = RegisteredPointer_getPointee;
                RegisteredPointer.prototype.destructor = RegisteredPointer_destructor;
                RegisteredPointer.prototype["argPackAdvance"] = 8;
                RegisteredPointer.prototype["readValueFromPointer"] = simpleReadValueFromPointer;
                RegisteredPointer.prototype["deleteObject"] = RegisteredPointer_deleteObject;
                RegisteredPointer.prototype["fromWireType"] = RegisteredPointer_fromWireType
            }

            function RegisteredPointer(name, registeredClass, isReference, isConst, isSmartPointer, pointeeType, sharingPolicy, rawGetPointee, rawConstructor, rawShare, rawDestructor) {
                this.name = name;
                this.registeredClass = registeredClass;
                this.isReference = isReference;
                this.isConst = isConst;
                this.isSmartPointer = isSmartPointer;
                this.pointeeType = pointeeType;
                this.sharingPolicy = sharingPolicy;
                this.rawGetPointee = rawGetPointee;
                this.rawConstructor = rawConstructor;
                this.rawShare = rawShare;
                this.rawDestructor = rawDestructor;
                if (!isSmartPointer && registeredClass.baseClass === undefined) {
                    if (isConst) {
                        this["toWireType"] = constNoSmartPtrRawPointerToWireType;
                        this.destructorFunction = null
                    } else {
                        this["toWireType"] = nonConstNoSmartPtrRawPointerToWireType;
                        this.destructorFunction = null
                    }
                } else {
                    this["toWireType"] = genericPointerToWireType
                }
            }

            function replacePublicSymbol(name, value, numArguments) {
                if (!Module.hasOwnProperty(name)) {
                    throwInternalError("Replacing nonexistant public symbol")
                }
                if (undefined !== Module[name].overloadTable && undefined !== numArguments) {
                    Module[name].overloadTable[numArguments] = value
                } else {
                    Module[name] = value;
                    Module[name].argCount = numArguments
                }
            }

            function dynCallLegacy(sig, ptr, args) {
                var f = Module["dynCall_" + sig];
                return args && args.length ? f.apply(null, [ptr].concat(args)) : f.call(null, ptr)
            }

            function dynCall(sig, ptr, args) {
                if (sig.includes("j")) {
                    return dynCallLegacy(sig, ptr, args)
                }
                return wasmTable.get(ptr).apply(null, args)
            }

            function getDynCaller(sig, ptr) {
                var argCache = [];
                return function () {
                    argCache.length = arguments.length;
                    for (var i = 0; i < arguments.length; i++) {
                        argCache[i] = arguments[i]
                    }
                    return dynCall(sig, ptr, argCache)
                }
            }

            function embind__requireFunction(signature, rawFunction) {
                signature = readLatin1String(signature);

                function makeDynCaller() {
                    if (signature.includes("j")) {
                        return getDynCaller(signature, rawFunction)
                    }
                    return wasmTable.get(rawFunction)
                }

                var fp = makeDynCaller();
                if (typeof fp !== "function") {
                    throwBindingError("unknown function pointer with signature " + signature + ": " + rawFunction)
                }
                return fp
            }

            var UnboundTypeError = undefined;

            function getTypeName(type) {
                var ptr = ___getTypeName(type);
                var rv = readLatin1String(ptr);
                _free(ptr);
                return rv
            }

            function throwUnboundTypeError(message, types) {
                var unboundTypes = [];
                var seen = {};

                function visit(type) {
                    if (seen[type]) {
                        return
                    }
                    if (registeredTypes[type]) {
                        return
                    }
                    if (typeDependencies[type]) {
                        typeDependencies[type].forEach(visit);
                        return
                    }
                    unboundTypes.push(type);
                    seen[type] = true
                }

                types.forEach(visit);
                throw new UnboundTypeError(message + ": " + unboundTypes.map(getTypeName).join([", "]))
            }

            function __embind_register_class(rawType, rawPointerType, rawConstPointerType, baseClassRawType, getActualTypeSignature, getActualType, upcastSignature, upcast, downcastSignature, downcast, name, destructorSignature, rawDestructor) {
                name = readLatin1String(name);
                getActualType = embind__requireFunction(getActualTypeSignature, getActualType);
                if (upcast) {
                    upcast = embind__requireFunction(upcastSignature, upcast)
                }
                if (downcast) {
                    downcast = embind__requireFunction(downcastSignature, downcast)
                }
                rawDestructor = embind__requireFunction(destructorSignature, rawDestructor);
                var legalFunctionName = makeLegalFunctionName(name);
                exposePublicSymbol(legalFunctionName, function () {
                    throwUnboundTypeError("Cannot construct " + name + " due to unbound types", [baseClassRawType])
                });
                whenDependentTypesAreResolved([rawType, rawPointerType, rawConstPointerType], baseClassRawType ? [baseClassRawType] : [], function (base) {
                    base = base[0];
                    var baseClass;
                    var basePrototype;
                    if (baseClassRawType) {
                        baseClass = base.registeredClass;
                        basePrototype = baseClass.instancePrototype
                    } else {
                        basePrototype = ClassHandle.prototype
                    }
                    var constructor = createNamedFunction(legalFunctionName, function () {
                        if (Object.getPrototypeOf(this) !== instancePrototype) {
                            throw new BindingError("Use 'new' to construct " + name)
                        }
                        if (undefined === registeredClass.constructor_body) {
                            throw new BindingError(name + " has no accessible constructor")
                        }
                        var body = registeredClass.constructor_body[arguments.length];
                        if (undefined === body) {
                            throw new BindingError("Tried to invoke ctor of " + name + " with invalid number of parameters (" + arguments.length + ") - expected (" + Object.keys(registeredClass.constructor_body).toString() + ") parameters instead!")
                        }
                        return body.apply(this, arguments)
                    });
                    var instancePrototype = Object.create(basePrototype, {constructor: {value: constructor}});
                    constructor.prototype = instancePrototype;
                    var registeredClass = new RegisteredClass(name, constructor, instancePrototype, rawDestructor, baseClass, getActualType, upcast, downcast);
                    var referenceConverter = new RegisteredPointer(name, registeredClass, true, false, false);
                    var pointerConverter = new RegisteredPointer(name + "*", registeredClass, false, false, false);
                    var constPointerConverter = new RegisteredPointer(name + " const*", registeredClass, false, true, false);
                    registeredPointers[rawType] = {
                        pointerType: pointerConverter,
                        constPointerType: constPointerConverter
                    };
                    replacePublicSymbol(legalFunctionName, constructor);
                    return [referenceConverter, pointerConverter, constPointerConverter]
                })
            }

            function new_(constructor, argumentList) {
                if (!(constructor instanceof Function)) {
                    throw new TypeError("new_ called with constructor type " + typeof constructor + " which is not a function")
                }
                var dummy = createNamedFunction(constructor.name || "unknownFunctionName", function () {
                });
                dummy.prototype = constructor.prototype;
                var obj = new dummy;
                var r = constructor.apply(obj, argumentList);
                return r instanceof Object ? r : obj
            }

            function craftInvokerFunction(humanName, argTypes, classType, cppInvokerFunc, cppTargetFunc) {
                var argCount = argTypes.length;
                if (argCount < 2) {
                    throwBindingError("argTypes array size mismatch! Must at least get return value and 'this' types!")
                }
                var isClassMethodFunc = argTypes[1] !== null && classType !== null;
                var needsDestructorStack = false;
                for (var i = 1; i < argTypes.length; ++i) {
                    if (argTypes[i] !== null && argTypes[i].destructorFunction === undefined) {
                        needsDestructorStack = true;
                        break
                    }
                }
                var returns = argTypes[0].name !== "void";
                var argsList = "";
                var argsListWired = "";
                for (var i = 0; i < argCount - 2; ++i) {
                    argsList += (i !== 0 ? ", " : "") + "arg" + i;
                    argsListWired += (i !== 0 ? ", " : "") + "arg" + i + "Wired"
                }
                var invokerFnBody = "return function " + makeLegalFunctionName(humanName) + "(" + argsList + ") {\n" + "if (arguments.length !== " + (argCount - 2) + ") {\n" + "throwBindingError('function " + humanName + " called with ' + arguments.length + ' arguments, expected " + (argCount - 2) + " args!');\n" + "}\n";
                if (needsDestructorStack) {
                    invokerFnBody += "var destructors = [];\n"
                }
                var dtorStack = needsDestructorStack ? "destructors" : "null";
                var args1 = ["throwBindingError", "invoker", "fn", "runDestructors", "retType", "classParam"];
                var args2 = [throwBindingError, cppInvokerFunc, cppTargetFunc, runDestructors, argTypes[0], argTypes[1]];
                if (isClassMethodFunc) {
                    invokerFnBody += "var thisWired = classParam.toWireType(" + dtorStack + ", this);\n"
                }
                for (var i = 0; i < argCount - 2; ++i) {
                    invokerFnBody += "var arg" + i + "Wired = argType" + i + ".toWireType(" + dtorStack + ", arg" + i + "); // " + argTypes[i + 2].name + "\n";
                    args1.push("argType" + i);
                    args2.push(argTypes[i + 2])
                }
                if (isClassMethodFunc) {
                    argsListWired = "thisWired" + (argsListWired.length > 0 ? ", " : "") + argsListWired
                }
                invokerFnBody += (returns ? "var rv = " : "") + "invoker(fn" + (argsListWired.length > 0 ? ", " : "") + argsListWired + ");\n";
                if (needsDestructorStack) {
                    invokerFnBody += "runDestructors(destructors);\n"
                } else {
                    for (var i = isClassMethodFunc ? 1 : 2; i < argTypes.length; ++i) {
                        var paramName = i === 1 ? "thisWired" : "arg" + (i - 2) + "Wired";
                        if (argTypes[i].destructorFunction !== null) {
                            invokerFnBody += paramName + "_dtor(" + paramName + "); // " + argTypes[i].name + "\n";
                            args1.push(paramName + "_dtor");
                            args2.push(argTypes[i].destructorFunction)
                        }
                    }
                }
                if (returns) {
                    invokerFnBody += "var ret = retType.fromWireType(rv);\n" + "return ret;\n"
                } else {
                }
                invokerFnBody += "}\n";
                args1.push(invokerFnBody);
                var invokerFunction = new_(Function, args1).apply(null, args2);
                return invokerFunction
            }

            function heap32VectorToArray(count, firstElement) {
                var array = [];
                for (var i = 0; i < count; i++) {
                    array.push(HEAP32[(firstElement >> 2) + i])
                }
                return array
            }

            function __embind_register_class_class_function(rawClassType, methodName, argCount, rawArgTypesAddr, invokerSignature, rawInvoker, fn) {
                var rawArgTypes = heap32VectorToArray(argCount, rawArgTypesAddr);
                methodName = readLatin1String(methodName);
                rawInvoker = embind__requireFunction(invokerSignature, rawInvoker);
                whenDependentTypesAreResolved([], [rawClassType], function (classType) {
                    classType = classType[0];
                    var humanName = classType.name + "." + methodName;

                    function unboundTypesHandler() {
                        throwUnboundTypeError("Cannot call " + humanName + " due to unbound types", rawArgTypes)
                    }

                    if (methodName.startsWith("@@")) {
                        methodName = Symbol[methodName.substring(2)]
                    }
                    var proto = classType.registeredClass.constructor;
                    if (undefined === proto[methodName]) {
                        unboundTypesHandler.argCount = argCount - 1;
                        proto[methodName] = unboundTypesHandler
                    } else {
                        ensureOverloadTable(proto, methodName, humanName);
                        proto[methodName].overloadTable[argCount - 1] = unboundTypesHandler
                    }
                    whenDependentTypesAreResolved([], rawArgTypes, function (argTypes) {
                        var invokerArgsArray = [argTypes[0], null].concat(argTypes.slice(1));
                        var func = craftInvokerFunction(humanName, invokerArgsArray, null, rawInvoker, fn);
                        if (undefined === proto[methodName].overloadTable) {
                            func.argCount = argCount - 1;
                            proto[methodName] = func
                        } else {
                            proto[methodName].overloadTable[argCount - 1] = func
                        }
                        return []
                    });
                    return []
                })
            }

            function validateThis(this_, classType, humanName) {
                if (!(this_ instanceof Object)) {
                    throwBindingError(humanName + ' with invalid "this": ' + this_)
                }
                if (!(this_ instanceof classType.registeredClass.constructor)) {
                    throwBindingError(humanName + ' incompatible with "this" of type ' + this_.constructor.name)
                }
                if (!this_.$$.ptr) {
                    throwBindingError("cannot call emscripten binding method " + humanName + " on deleted object")
                }
                return upcastPointer(this_.$$.ptr, this_.$$.ptrType.registeredClass, classType.registeredClass)
            }

            function __embind_register_class_class_property(rawClassType, fieldName, rawFieldType, rawFieldPtr, getterSignature, getter, setterSignature, setter) {
                fieldName = readLatin1String(fieldName);
                getter = embind__requireFunction(getterSignature, getter);
                whenDependentTypesAreResolved([], [rawClassType], function (classType) {
                    classType = classType[0];
                    var humanName = classType.name + "." + fieldName;
                    var desc = {
                        get: function () {
                            throwUnboundTypeError("Cannot access " + humanName + " due to unbound types", [rawFieldType])
                        }, enumerable: true, configurable: true
                    };
                    if (setter) {
                        desc.set = function () {
                            throwUnboundTypeError("Cannot access " + humanName + " due to unbound types", [rawFieldType])
                        }
                    } else {
                        desc.set = function (v) {
                            throwBindingError(humanName + " is a read-only property")
                        }
                    }
                    Object.defineProperty(classType.registeredClass.constructor, fieldName, desc);
                    whenDependentTypesAreResolved([], [rawFieldType], function (fieldType) {
                        fieldType = fieldType[0];
                        var desc = {
                            get: function () {
                                return fieldType["fromWireType"](getter(rawFieldPtr))
                            }, enumerable: true
                        };
                        if (setter) {
                            setter = embind__requireFunction(setterSignature, setter);
                            desc.set = function (v) {
                                var destructors = [];
                                setter(rawFieldPtr, fieldType["toWireType"](destructors, v));
                                runDestructors(destructors)
                            }
                        }
                        Object.defineProperty(classType.registeredClass.constructor, fieldName, desc);
                        return []
                    });
                    return []
                })
            }

            function __embind_register_class_constructor(rawClassType, argCount, rawArgTypesAddr, invokerSignature, invoker, rawConstructor) {
                assert(argCount > 0);
                var rawArgTypes = heap32VectorToArray(argCount, rawArgTypesAddr);
                invoker = embind__requireFunction(invokerSignature, invoker);
                whenDependentTypesAreResolved([], [rawClassType], function (classType) {
                    classType = classType[0];
                    var humanName = "constructor " + classType.name;
                    if (undefined === classType.registeredClass.constructor_body) {
                        classType.registeredClass.constructor_body = []
                    }
                    if (undefined !== classType.registeredClass.constructor_body[argCount - 1]) {
                        throw new BindingError("Cannot register multiple constructors with identical number of parameters (" + (argCount - 1) + ") for class '" + classType.name + "'! Overload resolution is currently only performed using the parameter count, not actual type info!")
                    }
                    classType.registeredClass.constructor_body[argCount - 1] = function unboundTypeHandler() {
                        throwUnboundTypeError("Cannot construct " + classType.name + " due to unbound types", rawArgTypes)
                    };
                    whenDependentTypesAreResolved([], rawArgTypes, function (argTypes) {
                        argTypes.splice(1, 0, null);
                        classType.registeredClass.constructor_body[argCount - 1] = craftInvokerFunction(humanName, argTypes, null, invoker, rawConstructor);
                        return []
                    });
                    return []
                })
            }

            function __embind_register_class_function(rawClassType, methodName, argCount, rawArgTypesAddr, invokerSignature, rawInvoker, context, isPureVirtual) {
                var rawArgTypes = heap32VectorToArray(argCount, rawArgTypesAddr);
                methodName = readLatin1String(methodName);
                rawInvoker = embind__requireFunction(invokerSignature, rawInvoker);
                whenDependentTypesAreResolved([], [rawClassType], function (classType) {
                    classType = classType[0];
                    var humanName = classType.name + "." + methodName;
                    if (methodName.startsWith("@@")) {
                        methodName = Symbol[methodName.substring(2)]
                    }
                    if (isPureVirtual) {
                        classType.registeredClass.pureVirtualFunctions.push(methodName)
                    }

                    function unboundTypesHandler() {
                        throwUnboundTypeError("Cannot call " + humanName + " due to unbound types", rawArgTypes)
                    }

                    var proto = classType.registeredClass.instancePrototype;
                    var method = proto[methodName];
                    if (undefined === method || undefined === method.overloadTable && method.className !== classType.name && method.argCount === argCount - 2) {
                        unboundTypesHandler.argCount = argCount - 2;
                        unboundTypesHandler.className = classType.name;
                        proto[methodName] = unboundTypesHandler
                    } else {
                        ensureOverloadTable(proto, methodName, humanName);
                        proto[methodName].overloadTable[argCount - 2] = unboundTypesHandler
                    }
                    whenDependentTypesAreResolved([], rawArgTypes, function (argTypes) {
                        var memberFunction = craftInvokerFunction(humanName, argTypes, classType, rawInvoker, context);
                        if (undefined === proto[methodName].overloadTable) {
                            memberFunction.argCount = argCount - 2;
                            proto[methodName] = memberFunction
                        } else {
                            proto[methodName].overloadTable[argCount - 2] = memberFunction
                        }
                        return []
                    });
                    return []
                })
            }

            function __embind_register_class_property(classType, fieldName, getterReturnType, getterSignature, getter, getterContext, setterArgumentType, setterSignature, setter, setterContext) {
                fieldName = readLatin1String(fieldName);
                getter = embind__requireFunction(getterSignature, getter);
                whenDependentTypesAreResolved([], [classType], function (classType) {
                    classType = classType[0];
                    var humanName = classType.name + "." + fieldName;
                    var desc = {
                        get: function () {
                            throwUnboundTypeError("Cannot access " + humanName + " due to unbound types", [getterReturnType, setterArgumentType])
                        }, enumerable: true, configurable: true
                    };
                    if (setter) {
                        desc.set = function () {
                            throwUnboundTypeError("Cannot access " + humanName + " due to unbound types", [getterReturnType, setterArgumentType])
                        }
                    } else {
                        desc.set = function (v) {
                            throwBindingError(humanName + " is a read-only property")
                        }
                    }
                    Object.defineProperty(classType.registeredClass.instancePrototype, fieldName, desc);
                    whenDependentTypesAreResolved([], setter ? [getterReturnType, setterArgumentType] : [getterReturnType], function (types) {
                        var getterReturnType = types[0];
                        var desc = {
                            get: function () {
                                var ptr = validateThis(this, classType, humanName + " getter");
                                return getterReturnType["fromWireType"](getter(getterContext, ptr))
                            }, enumerable: true
                        };
                        if (setter) {
                            setter = embind__requireFunction(setterSignature, setter);
                            var setterArgumentType = types[1];
                            desc.set = function (v) {
                                var ptr = validateThis(this, classType, humanName + " setter");
                                var destructors = [];
                                setter(setterContext, ptr, setterArgumentType["toWireType"](destructors, v));
                                runDestructors(destructors)
                            }
                        }
                        Object.defineProperty(classType.registeredClass.instancePrototype, fieldName, desc);
                        return []
                    });
                    return []
                })
            }

            function __embind_register_constant(name, type, value) {
                name = readLatin1String(name);
                whenDependentTypesAreResolved([], [type], function (type) {
                    type = type[0];
                    Module[name] = type["fromWireType"](value);
                    return []
                })
            }

            var emval_free_list = [];
            var emval_handle_array = [{}, {value: undefined}, {value: null}, {value: true}, {value: false}];

            function __emval_decref(handle) {
                if (handle > 4 && 0 === --emval_handle_array[handle].refcount) {
                    emval_handle_array[handle] = undefined;
                    emval_free_list.push(handle)
                }
            }

            function count_emval_handles() {
                var count = 0;
                for (var i = 5; i < emval_handle_array.length; ++i) {
                    if (emval_handle_array[i] !== undefined) {
                        ++count
                    }
                }
                return count
            }

            function get_first_emval() {
                for (var i = 5; i < emval_handle_array.length; ++i) {
                    if (emval_handle_array[i] !== undefined) {
                        return emval_handle_array[i]
                    }
                }
                return null
            }

            function init_emval() {
                Module["count_emval_handles"] = count_emval_handles;
                Module["get_first_emval"] = get_first_emval
            }

            function __emval_register(value) {
                switch (value) {
                    case undefined: {
                        return 1
                    }
                    case null: {
                        return 2
                    }
                    case true: {
                        return 3
                    }
                    case false: {
                        return 4
                    }
                    default: {
                        var handle = emval_free_list.length ? emval_free_list.pop() : emval_handle_array.length;
                        emval_handle_array[handle] = {refcount: 1, value: value};
                        return handle
                    }
                }
            }

            function __embind_register_emval(rawType, name) {
                name = readLatin1String(name);
                registerType(rawType, {
                    name: name, "fromWireType": function (handle) {
                        var rv = emval_handle_array[handle].value;
                        __emval_decref(handle);
                        return rv
                    }, "toWireType": function (destructors, value) {
                        return __emval_register(value)
                    }, "argPackAdvance": 8, "readValueFromPointer": simpleReadValueFromPointer, destructorFunction: null
                })
            }

            function enumReadValueFromPointer(name, shift, signed) {
                switch (shift) {
                    case 0:
                        return function (pointer) {
                            var heap = signed ? HEAP8 : HEAPU8;
                            return this["fromWireType"](heap[pointer])
                        };
                    case 1:
                        return function (pointer) {
                            var heap = signed ? HEAP16 : HEAPU16;
                            return this["fromWireType"](heap[pointer >> 1])
                        };
                    case 2:
                        return function (pointer) {
                            var heap = signed ? HEAP32 : HEAPU32;
                            return this["fromWireType"](heap[pointer >> 2])
                        };
                    default:
                        throw new TypeError("Unknown integer type: " + name)
                }
            }

            function __embind_register_enum(rawType, name, size, isSigned) {
                var shift = getShiftFromSize(size);
                name = readLatin1String(name);

                function ctor() {
                }

                ctor.values = {};
                registerType(rawType, {
                    name: name,
                    constructor: ctor,
                    "fromWireType": function (c) {
                        return this.constructor.values[c]
                    },
                    "toWireType": function (destructors, c) {
                        return c.value
                    },
                    "argPackAdvance": 8,
                    "readValueFromPointer": enumReadValueFromPointer(name, shift, isSigned),
                    destructorFunction: null
                });
                exposePublicSymbol(name, ctor)
            }

            function requireRegisteredType(rawType, humanName) {
                var impl = registeredTypes[rawType];
                if (undefined === impl) {
                    throwBindingError(humanName + " has unknown type " + getTypeName(rawType))
                }
                return impl
            }

            function __embind_register_enum_value(rawEnumType, name, enumValue) {
                var enumType = requireRegisteredType(rawEnumType, "enum");
                name = readLatin1String(name);
                var Enum = enumType.constructor;
                var Value = Object.create(enumType.constructor.prototype, {
                    value: {value: enumValue},
                    constructor: {
                        value: createNamedFunction(enumType.name + "_" + name, function () {
                        })
                    }
                });
                Enum.values[enumValue] = Value;
                Enum[name] = Value
            }

            function _embind_repr(v) {
                if (v === null) {
                    return "null"
                }
                var t = typeof v;
                if (t === "object" || t === "array" || t === "function") {
                    return v.toString()
                } else {
                    return "" + v
                }
            }

            function floatReadValueFromPointer(name, shift) {
                switch (shift) {
                    case 2:
                        return function (pointer) {
                            return this["fromWireType"](HEAPF32[pointer >> 2])
                        };
                    case 3:
                        return function (pointer) {
                            return this["fromWireType"](HEAPF64[pointer >> 3])
                        };
                    default:
                        throw new TypeError("Unknown float type: " + name)
                }
            }

            function __embind_register_float(rawType, name, size) {
                var shift = getShiftFromSize(size);
                name = readLatin1String(name);
                registerType(rawType, {
                    name: name,
                    "fromWireType": function (value) {
                        return value
                    },
                    "toWireType": function (destructors, value) {
                        if (typeof value !== "number" && typeof value !== "boolean") {
                            throw new TypeError('Cannot convert "' + _embind_repr(value) + '" to ' + this.name)
                        }
                        return value
                    },
                    "argPackAdvance": 8,
                    "readValueFromPointer": floatReadValueFromPointer(name, shift),
                    destructorFunction: null
                })
            }

            function __embind_register_function(name, argCount, rawArgTypesAddr, signature, rawInvoker, fn) {
                var argTypes = heap32VectorToArray(argCount, rawArgTypesAddr);
                name = readLatin1String(name);
                rawInvoker = embind__requireFunction(signature, rawInvoker);
                exposePublicSymbol(name, function () {
                    throwUnboundTypeError("Cannot call " + name + " due to unbound types", argTypes)
                }, argCount - 1);
                whenDependentTypesAreResolved([], argTypes, function (argTypes) {
                    var invokerArgsArray = [argTypes[0], null].concat(argTypes.slice(1));
                    replacePublicSymbol(name, craftInvokerFunction(name, invokerArgsArray, null, rawInvoker, fn), argCount - 1);
                    return []
                })
            }

            function integerReadValueFromPointer(name, shift, signed) {
                switch (shift) {
                    case 0:
                        return signed ? function readS8FromPointer(pointer) {
                            return HEAP8[pointer]
                        } : function readU8FromPointer(pointer) {
                            return HEAPU8[pointer]
                        };
                    case 1:
                        return signed ? function readS16FromPointer(pointer) {
                            return HEAP16[pointer >> 1]
                        } : function readU16FromPointer(pointer) {
                            return HEAPU16[pointer >> 1]
                        };
                    case 2:
                        return signed ? function readS32FromPointer(pointer) {
                            return HEAP32[pointer >> 2]
                        } : function readU32FromPointer(pointer) {
                            return HEAPU32[pointer >> 2]
                        };
                    default:
                        throw new TypeError("Unknown integer type: " + name)
                }
            }

            function __embind_register_integer(primitiveType, name, size, minRange, maxRange) {
                name = readLatin1String(name);
                if (maxRange === -1) {
                    maxRange = 4294967295
                }
                var shift = getShiftFromSize(size);
                var fromWireType = function (value) {
                    return value
                };
                if (minRange === 0) {
                    var bitshift = 32 - 8 * size;
                    fromWireType = function (value) {
                        return value << bitshift >>> bitshift
                    }
                }
                var isUnsignedType = name.includes("unsigned");
                registerType(primitiveType, {
                    name: name,
                    "fromWireType": fromWireType,
                    "toWireType": function (destructors, value) {
                        if (typeof value !== "number" && typeof value !== "boolean") {
                            throw new TypeError('Cannot convert "' + _embind_repr(value) + '" to ' + this.name)
                        }
                        if (value < minRange || value > maxRange) {
                            throw new TypeError('Passing a number "' + _embind_repr(value) + '" from JS side to C/C++ side to an argument of type "' + name + '", which is outside the valid range [' + minRange + ", " + maxRange + "]!")
                        }
                        return isUnsignedType ? value >>> 0 : value | 0
                    },
                    "argPackAdvance": 8,
                    "readValueFromPointer": integerReadValueFromPointer(name, shift, minRange !== 0),
                    destructorFunction: null
                })
            }

            function __embind_register_memory_view(rawType, dataTypeIndex, name) {
                var typeMapping = [Int8Array, Uint8Array, Int16Array, Uint16Array, Int32Array, Uint32Array, Float32Array, Float64Array];
                var TA = typeMapping[dataTypeIndex];

                function decodeMemoryView(handle) {
                    handle = handle >> 2;
                    var heap = HEAPU32;
                    var size = heap[handle];
                    var data = heap[handle + 1];
                    return new TA(buffer, data, size)
                }

                name = readLatin1String(name);
                registerType(rawType, {
                    name: name,
                    "fromWireType": decodeMemoryView,
                    "argPackAdvance": 8,
                    "readValueFromPointer": decodeMemoryView
                }, {ignoreDuplicateRegistrations: true})
            }

            function __embind_register_smart_ptr(rawType, rawPointeeType, name, sharingPolicy, getPointeeSignature, rawGetPointee, constructorSignature, rawConstructor, shareSignature, rawShare, destructorSignature, rawDestructor) {
                name = readLatin1String(name);
                rawGetPointee = embind__requireFunction(getPointeeSignature, rawGetPointee);
                rawConstructor = embind__requireFunction(constructorSignature, rawConstructor);
                rawShare = embind__requireFunction(shareSignature, rawShare);
                rawDestructor = embind__requireFunction(destructorSignature, rawDestructor);
                whenDependentTypesAreResolved([rawType], [rawPointeeType], function (pointeeType) {
                    pointeeType = pointeeType[0];
                    var registeredPointer = new RegisteredPointer(name, pointeeType.registeredClass, false, false, true, pointeeType, sharingPolicy, rawGetPointee, rawConstructor, rawShare, rawDestructor);
                    return [registeredPointer]
                })
            }

            function __embind_register_std_string(rawType, name) {
                name = readLatin1String(name);
                var stdStringIsUTF8 = name === "std::string";
                registerType(rawType, {
                    name: name,
                    "fromWireType": function (value) {
                        var length = HEAPU32[value >> 2];
                        var str;
                        if (stdStringIsUTF8) {
                            var decodeStartPtr = value + 4;
                            for (var i = 0; i <= length; ++i) {
                                var currentBytePtr = value + 4 + i;
                                if (i == length || HEAPU8[currentBytePtr] == 0) {
                                    var maxRead = currentBytePtr - decodeStartPtr;
                                    var stringSegment = UTF8ToString(decodeStartPtr, maxRead);
                                    if (str === undefined) {
                                        str = stringSegment
                                    } else {
                                        str += String.fromCharCode(0);
                                        str += stringSegment
                                    }
                                    decodeStartPtr = currentBytePtr + 1
                                }
                            }
                        } else {
                            var a = new Array(length);
                            for (var i = 0; i < length; ++i) {
                                a[i] = String.fromCharCode(HEAPU8[value + 4 + i])
                            }
                            str = a.join("")
                        }
                        _free(value);
                        return str
                    },
                    "toWireType": function (destructors, value) {
                        if (value instanceof ArrayBuffer) {
                            value = new Uint8Array(value)
                        }
                        var getLength;
                        var valueIsOfTypeString = typeof value === "string";
                        if (!(valueIsOfTypeString || value instanceof Uint8Array || value instanceof Uint8ClampedArray || value instanceof Int8Array)) {
                            throwBindingError("Cannot pass non-string to std::string")
                        }
                        if (stdStringIsUTF8 && valueIsOfTypeString) {
                            getLength = function () {
                                return lengthBytesUTF8(value)
                            }
                        } else {
                            getLength = function () {
                                return value.length
                            }
                        }
                        var length = getLength();
                        var ptr = _malloc(4 + length + 1);
                        HEAPU32[ptr >> 2] = length;
                        if (stdStringIsUTF8 && valueIsOfTypeString) {
                            stringToUTF8(value, ptr + 4, length + 1)
                        } else {
                            if (valueIsOfTypeString) {
                                for (var i = 0; i < length; ++i) {
                                    var charCode = value.charCodeAt(i);
                                    if (charCode > 255) {
                                        _free(ptr);
                                        throwBindingError("String has UTF-16 code units that do not fit in 8 bits")
                                    }
                                    HEAPU8[ptr + 4 + i] = charCode
                                }
                            } else {
                                for (var i = 0; i < length; ++i) {
                                    HEAPU8[ptr + 4 + i] = value[i]
                                }
                            }
                        }
                        if (destructors !== null) {
                            destructors.push(_free, ptr)
                        }
                        return ptr
                    },
                    "argPackAdvance": 8,
                    "readValueFromPointer": simpleReadValueFromPointer,
                    destructorFunction: function (ptr) {
                        _free(ptr)
                    }
                })
            }

            function __embind_register_std_wstring(rawType, charSize, name) {
                name = readLatin1String(name);
                var decodeString, encodeString, getHeap, lengthBytesUTF, shift;
                if (charSize === 2) {
                    decodeString = UTF16ToString;
                    encodeString = stringToUTF16;
                    lengthBytesUTF = lengthBytesUTF16;
                    getHeap = function () {
                        return HEAPU16
                    };
                    shift = 1
                } else if (charSize === 4) {
                    decodeString = UTF32ToString;
                    encodeString = stringToUTF32;
                    lengthBytesUTF = lengthBytesUTF32;
                    getHeap = function () {
                        return HEAPU32
                    };
                    shift = 2
                }
                registerType(rawType, {
                    name: name,
                    "fromWireType": function (value) {
                        var length = HEAPU32[value >> 2];
                        var HEAP = getHeap();
                        var str;
                        var decodeStartPtr = value + 4;
                        for (var i = 0; i <= length; ++i) {
                            var currentBytePtr = value + 4 + i * charSize;
                            if (i == length || HEAP[currentBytePtr >> shift] == 0) {
                                var maxReadBytes = currentBytePtr - decodeStartPtr;
                                var stringSegment = decodeString(decodeStartPtr, maxReadBytes);
                                if (str === undefined) {
                                    str = stringSegment
                                } else {
                                    str += String.fromCharCode(0);
                                    str += stringSegment
                                }
                                decodeStartPtr = currentBytePtr + charSize
                            }
                        }
                        _free(value);
                        return str
                    },
                    "toWireType": function (destructors, value) {
                        if (!(typeof value === "string")) {
                            throwBindingError("Cannot pass non-string to C++ string type " + name)
                        }
                        var length = lengthBytesUTF(value);
                        var ptr = _malloc(4 + length + charSize);
                        HEAPU32[ptr >> 2] = length >> shift;
                        encodeString(value, ptr + 4, length + charSize);
                        if (destructors !== null) {
                            destructors.push(_free, ptr)
                        }
                        return ptr
                    },
                    "argPackAdvance": 8,
                    "readValueFromPointer": simpleReadValueFromPointer,
                    destructorFunction: function (ptr) {
                        _free(ptr)
                    }
                })
            }

            function __embind_register_value_array(rawType, name, constructorSignature, rawConstructor, destructorSignature, rawDestructor) {
                tupleRegistrations[rawType] = {
                    name: readLatin1String(name),
                    rawConstructor: embind__requireFunction(constructorSignature, rawConstructor),
                    rawDestructor: embind__requireFunction(destructorSignature, rawDestructor),
                    elements: []
                }
            }

            function __embind_register_value_array_element(rawTupleType, getterReturnType, getterSignature, getter, getterContext, setterArgumentType, setterSignature, setter, setterContext) {
                tupleRegistrations[rawTupleType].elements.push({
                    getterReturnType: getterReturnType,
                    getter: embind__requireFunction(getterSignature, getter),
                    getterContext: getterContext,
                    setterArgumentType: setterArgumentType,
                    setter: embind__requireFunction(setterSignature, setter),
                    setterContext: setterContext
                })
            }

            function __embind_register_void(rawType, name) {
                name = readLatin1String(name);
                registerType(rawType, {
                    isVoid: true, name: name, "argPackAdvance": 0, "fromWireType": function () {
                        return undefined
                    }, "toWireType": function (destructors, o) {
                        return undefined
                    }
                })
            }

            function requireHandle(handle) {
                if (!handle) {
                    throwBindingError("Cannot use deleted val. handle = " + handle)
                }
                return emval_handle_array[handle].value
            }

            function __emval_as(handle, returnType, destructorsRef) {
                handle = requireHandle(handle);
                returnType = requireRegisteredType(returnType, "emval::as");
                var destructors = [];
                var rd = __emval_register(destructors);
                HEAP32[destructorsRef >> 2] = rd;
                return returnType["toWireType"](destructors, handle)
            }

            function __emval_lookupTypes(argCount, argTypes) {
                var a = new Array(argCount);
                for (var i = 0; i < argCount; ++i) {
                    a[i] = requireRegisteredType(HEAP32[(argTypes >> 2) + i], "parameter " + i)
                }
                return a
            }

            function __emval_call(handle, argCount, argTypes, argv) {
                handle = requireHandle(handle);
                var types = __emval_lookupTypes(argCount, argTypes);
                var args = new Array(argCount);
                for (var i = 0; i < argCount; ++i) {
                    var type = types[i];
                    args[i] = type["readValueFromPointer"](argv);
                    argv += type["argPackAdvance"]
                }
                var rv = handle.apply(undefined, args);
                return __emval_register(rv)
            }

            var emval_symbols = {};

            function getStringOrSymbol(address) {
                var symbol = emval_symbols[address];
                if (symbol === undefined) {
                    return readLatin1String(address)
                } else {
                    return symbol
                }
            }

            var emval_methodCallers = [];

            function __emval_call_void_method(caller, handle, methodName, args) {
                caller = emval_methodCallers[caller];
                handle = requireHandle(handle);
                methodName = getStringOrSymbol(methodName);
                caller(handle, methodName, null, args)
            }

            function __emval_addMethodCaller(caller) {
                var id = emval_methodCallers.length;
                emval_methodCallers.push(caller);
                return id
            }

            function __emval_get_method_caller(argCount, argTypes) {
                var types = __emval_lookupTypes(argCount, argTypes);
                var retType = types[0];
                var signatureName = retType.name + "_$" + types.slice(1).map(function (t) {
                    return t.name
                }).join("_") + "$";
                var params = ["retType"];
                var args = [retType];
                var argsList = "";
                for (var i = 0; i < argCount - 1; ++i) {
                    argsList += (i !== 0 ? ", " : "") + "arg" + i;
                    params.push("argType" + i);
                    args.push(types[1 + i])
                }
                var functionName = makeLegalFunctionName("methodCaller_" + signatureName);
                var functionBody = "return function " + functionName + "(handle, name, destructors, args) {\n";
                var offset = 0;
                for (var i = 0; i < argCount - 1; ++i) {
                    functionBody += "    var arg" + i + " = argType" + i + ".readValueFromPointer(args" + (offset ? "+" + offset : "") + ");\n";
                    offset += types[i + 1]["argPackAdvance"]
                }
                functionBody += "    var rv = handle[name](" + argsList + ");\n";
                for (var i = 0; i < argCount - 1; ++i) {
                    if (types[i + 1]["deleteObject"]) {
                        functionBody += "    argType" + i + ".deleteObject(arg" + i + ");\n"
                    }
                }
                if (!retType.isVoid) {
                    functionBody += "    return retType.toWireType(destructors, rv);\n"
                }
                functionBody += "};\n";
                params.push(functionBody);
                var invokerFunction = new_(Function, params).apply(null, args);
                return __emval_addMethodCaller(invokerFunction)
            }

            function __emval_get_property(handle, key) {
                handle = requireHandle(handle);
                key = requireHandle(key);
                return __emval_register(handle[key])
            }

            function __emval_incref(handle) {
                if (handle > 4) {
                    emval_handle_array[handle].refcount += 1
                }
            }

            function __emval_new_array() {
                return __emval_register([])
            }

            function __emval_new_cstring(v) {
                return __emval_register(getStringOrSymbol(v))
            }

            function __emval_new_object() {
                return __emval_register({})
            }

            function __emval_run_destructors(handle) {
                var destructors = emval_handle_array[handle].value;
                runDestructors(destructors);
                __emval_decref(handle)
            }

            function __emval_set_property(handle, key, value) {
                handle = requireHandle(handle);
                key = requireHandle(key);
                value = requireHandle(value);
                handle[key] = value
            }

            function __emval_take_value(type, argv) {
                type = requireRegisteredType(type, "_emval_take_value");
                var v = type["readValueFromPointer"](argv);
                return __emval_register(v)
            }

            function __emval_typeof(handle) {
                handle = requireHandle(handle);
                return __emval_register(typeof handle)
            }

            function _abort() {
                abort()
            }

            var readAsmConstArgsArray = [];

            function readAsmConstArgs(sigPtr, buf) {
                readAsmConstArgsArray.length = 0;
                var ch;
                buf >>= 2;
                while (ch = HEAPU8[sigPtr++]) {
                    var double = ch < 105;
                    if (double && buf & 1) buf++;
                    readAsmConstArgsArray.push(double ? HEAPF64[buf++ >> 1] : HEAP32[buf]);
                    ++buf
                }
                return readAsmConstArgsArray
            }

            function _emscripten_asm_const_int(code, sigPtr, argbuf) {
                var args = readAsmConstArgs(sigPtr, argbuf);
                return ASM_CONSTS[code].apply(null, args)
            }

            function _emscripten_memcpy_big(dest, src, num) {
                HEAPU8.copyWithin(dest, src, src + num)
            }

            function emscripten_realloc_buffer(size) {
                try {
                    wasmMemory.grow(size - buffer.byteLength + 65535 >>> 16);
                    updateGlobalBufferAndViews(wasmMemory.buffer);
                    return 1
                } catch (e) {
                }
            }

            function _emscripten_resize_heap(requestedSize) {
                var oldSize = HEAPU8.length;
                requestedSize = requestedSize >>> 0;
                var maxHeapSize = 2147483648;
                if (requestedSize > maxHeapSize) {
                    return false
                }
                for (var cutDown = 1; cutDown <= 4; cutDown *= 2) {
                    var overGrownHeapSize = oldSize + 33554432 / cutDown;
                    var newSize = Math.min(maxHeapSize, alignUp(Math.max(requestedSize, overGrownHeapSize), 65536));
                    var replacement = emscripten_realloc_buffer(newSize);
                    if (replacement) {
                        return true
                    }
                }
                return false
            }

            var ENV = {};

            function getExecutableName() {
                return thisProgram || "./this.program"
            }

            function getEnvStrings() {
                if (!getEnvStrings.strings) {
                    var lang = (typeof navigator === "object" && navigator.languages && navigator.languages[0] || "C").replace("-", "_") + ".UTF-8";
                    var env = {
                        "USER": "web_user",
                        "LOGNAME": "web_user",
                        "PATH": "/",
                        "PWD": "/",
                        "HOME": "/home/web_user",
                        "LANG": lang,
                        "_": getExecutableName()
                    };
                    for (var x in ENV) {
                        if (ENV[x] === undefined) delete env[x]; else env[x] = ENV[x]
                    }
                    var strings = [];
                    for (var x in env) {
                        strings.push(x + "=" + env[x])
                    }
                    getEnvStrings.strings = strings
                }
                return getEnvStrings.strings
            }

            function _environ_get(__environ, environ_buf) {
                var bufSize = 0;
                getEnvStrings().forEach(function (string, i) {
                    var ptr = environ_buf + bufSize;
                    HEAP32[__environ + i * 4 >> 2] = ptr;
                    writeAsciiToMemory(string, ptr);
                    bufSize += string.length + 1
                });
                return 0
            }

            function _environ_sizes_get(penviron_count, penviron_buf_size) {
                var strings = getEnvStrings();
                HEAP32[penviron_count >> 2] = strings.length;
                var bufSize = 0;
                strings.forEach(function (string) {
                    bufSize += string.length + 1
                });
                HEAP32[penviron_buf_size >> 2] = bufSize;
                return 0
            }

            function _fd_close(fd) {
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    FS.close(stream);
                    return 0
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return e.errno
                }
            }

            function _fd_read(fd, iov, iovcnt, pnum) {
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    var num = SYSCALLS.doReadv(stream, iov, iovcnt);
                    HEAP32[pnum >> 2] = num;
                    return 0
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return e.errno
                }
            }

            function _fd_seek(fd, offset_low, offset_high, whence, newOffset) {
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    var HIGH_OFFSET = 4294967296;
                    var offset = offset_high * HIGH_OFFSET + (offset_low >>> 0);
                    var DOUBLE_LIMIT = 9007199254740992;
                    if (offset <= -DOUBLE_LIMIT || offset >= DOUBLE_LIMIT) {
                        return -61
                    }
                    FS.llseek(stream, offset, whence);
                    tempI64 = [stream.position >>> 0, (tempDouble = stream.position, +Math.abs(tempDouble) >= 1 ? tempDouble > 0 ? (Math.min(+Math.floor(tempDouble / 4294967296), 4294967295) | 0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296) >>> 0 : 0)], HEAP32[newOffset >> 2] = tempI64[0], HEAP32[newOffset + 4 >> 2] = tempI64[1];
                    if (stream.getdents && offset === 0 && whence === 0) stream.getdents = null;
                    return 0
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return e.errno
                }
            }

            function _fd_write(fd, iov, iovcnt, pnum) {
                try {
                    var stream = SYSCALLS.getStreamFromFD(fd);
                    var num = SYSCALLS.doWritev(stream, iov, iovcnt);
                    HEAP32[pnum >> 2] = num;
                    return 0
                } catch (e) {
                    if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
                    return e.errno
                }
            }

            function _getTempRet0() {
                return getTempRet0()
            }

            function _llvm_eh_typeid_for(type) {
                return type
            }

            function _setTempRet0(val) {
                setTempRet0(val)
            }

            function __isLeapYear(year) {
                return year % 4 === 0 && (year % 100 !== 0 || year % 400 === 0)
            }

            function __arraySum(array, index) {
                var sum = 0;
                for (var i = 0; i <= index; sum += array[i++]) {
                }
                return sum
            }

            var __MONTH_DAYS_LEAP = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
            var __MONTH_DAYS_REGULAR = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];

            function __addDays(date, days) {
                var newDate = new Date(date.getTime());
                while (days > 0) {
                    var leap = __isLeapYear(newDate.getFullYear());
                    var currentMonth = newDate.getMonth();
                    var daysInCurrentMonth = (leap ? __MONTH_DAYS_LEAP : __MONTH_DAYS_REGULAR)[currentMonth];
                    if (days > daysInCurrentMonth - newDate.getDate()) {
                        days -= daysInCurrentMonth - newDate.getDate() + 1;
                        newDate.setDate(1);
                        if (currentMonth < 11) {
                            newDate.setMonth(currentMonth + 1)
                        } else {
                            newDate.setMonth(0);
                            newDate.setFullYear(newDate.getFullYear() + 1)
                        }
                    } else {
                        newDate.setDate(newDate.getDate() + days);
                        return newDate
                    }
                }
                return newDate
            }

            function _strftime(s, maxsize, format, tm) {
                var tm_zone = HEAP32[tm + 40 >> 2];
                var date = {
                    tm_sec: HEAP32[tm >> 2],
                    tm_min: HEAP32[tm + 4 >> 2],
                    tm_hour: HEAP32[tm + 8 >> 2],
                    tm_mday: HEAP32[tm + 12 >> 2],
                    tm_mon: HEAP32[tm + 16 >> 2],
                    tm_year: HEAP32[tm + 20 >> 2],
                    tm_wday: HEAP32[tm + 24 >> 2],
                    tm_yday: HEAP32[tm + 28 >> 2],
                    tm_isdst: HEAP32[tm + 32 >> 2],
                    tm_gmtoff: HEAP32[tm + 36 >> 2],
                    tm_zone: tm_zone ? UTF8ToString(tm_zone) : ""
                };
                var pattern = UTF8ToString(format);
                var EXPANSION_RULES_1 = {
                    "%c": "%a %b %d %H:%M:%S %Y",
                    "%D": "%m/%d/%y",
                    "%F": "%Y-%m-%d",
                    "%h": "%b",
                    "%r": "%I:%M:%S %p",
                    "%R": "%H:%M",
                    "%T": "%H:%M:%S",
                    "%x": "%m/%d/%y",
                    "%X": "%H:%M:%S",
                    "%Ec": "%c",
                    "%EC": "%C",
                    "%Ex": "%m/%d/%y",
                    "%EX": "%H:%M:%S",
                    "%Ey": "%y",
                    "%EY": "%Y",
                    "%Od": "%d",
                    "%Oe": "%e",
                    "%OH": "%H",
                    "%OI": "%I",
                    "%Om": "%m",
                    "%OM": "%M",
                    "%OS": "%S",
                    "%Ou": "%u",
                    "%OU": "%U",
                    "%OV": "%V",
                    "%Ow": "%w",
                    "%OW": "%W",
                    "%Oy": "%y"
                };
                for (var rule in EXPANSION_RULES_1) {
                    pattern = pattern.replace(new RegExp(rule, "g"), EXPANSION_RULES_1[rule])
                }
                var WEEKDAYS = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
                var MONTHS = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];

                function leadingSomething(value, digits, character) {
                    var str = typeof value === "number" ? value.toString() : value || "";
                    while (str.length < digits) {
                        str = character[0] + str
                    }
                    return str
                }

                function leadingNulls(value, digits) {
                    return leadingSomething(value, digits, "0")
                }

                function compareByDay(date1, date2) {
                    function sgn(value) {
                        return value < 0 ? -1 : value > 0 ? 1 : 0
                    }

                    var compare;
                    if ((compare = sgn(date1.getFullYear() - date2.getFullYear())) === 0) {
                        if ((compare = sgn(date1.getMonth() - date2.getMonth())) === 0) {
                            compare = sgn(date1.getDate() - date2.getDate())
                        }
                    }
                    return compare
                }

                function getFirstWeekStartDate(janFourth) {
                    switch (janFourth.getDay()) {
                        case 0:
                            return new Date(janFourth.getFullYear() - 1, 11, 29);
                        case 1:
                            return janFourth;
                        case 2:
                            return new Date(janFourth.getFullYear(), 0, 3);
                        case 3:
                            return new Date(janFourth.getFullYear(), 0, 2);
                        case 4:
                            return new Date(janFourth.getFullYear(), 0, 1);
                        case 5:
                            return new Date(janFourth.getFullYear() - 1, 11, 31);
                        case 6:
                            return new Date(janFourth.getFullYear() - 1, 11, 30)
                    }
                }

                function getWeekBasedYear(date) {
                    var thisDate = __addDays(new Date(date.tm_year + 1900, 0, 1), date.tm_yday);
                    var janFourthThisYear = new Date(thisDate.getFullYear(), 0, 4);
                    var janFourthNextYear = new Date(thisDate.getFullYear() + 1, 0, 4);
                    var firstWeekStartThisYear = getFirstWeekStartDate(janFourthThisYear);
                    var firstWeekStartNextYear = getFirstWeekStartDate(janFourthNextYear);
                    if (compareByDay(firstWeekStartThisYear, thisDate) <= 0) {
                        if (compareByDay(firstWeekStartNextYear, thisDate) <= 0) {
                            return thisDate.getFullYear() + 1
                        } else {
                            return thisDate.getFullYear()
                        }
                    } else {
                        return thisDate.getFullYear() - 1
                    }
                }

                var EXPANSION_RULES_2 = {
                    "%a": function (date) {
                        return WEEKDAYS[date.tm_wday].substring(0, 3)
                    }, "%A": function (date) {
                        return WEEKDAYS[date.tm_wday]
                    }, "%b": function (date) {
                        return MONTHS[date.tm_mon].substring(0, 3)
                    }, "%B": function (date) {
                        return MONTHS[date.tm_mon]
                    }, "%C": function (date) {
                        var year = date.tm_year + 1900;
                        return leadingNulls(year / 100 | 0, 2)
                    }, "%d": function (date) {
                        return leadingNulls(date.tm_mday, 2)
                    }, "%e": function (date) {
                        return leadingSomething(date.tm_mday, 2, " ")
                    }, "%g": function (date) {
                        return getWeekBasedYear(date).toString().substring(2)
                    }, "%G": function (date) {
                        return getWeekBasedYear(date)
                    }, "%H": function (date) {
                        return leadingNulls(date.tm_hour, 2)
                    }, "%I": function (date) {
                        var twelveHour = date.tm_hour;
                        if (twelveHour == 0) twelveHour = 12; else if (twelveHour > 12) twelveHour -= 12;
                        return leadingNulls(twelveHour, 2)
                    }, "%j": function (date) {
                        return leadingNulls(date.tm_mday + __arraySum(__isLeapYear(date.tm_year + 1900) ? __MONTH_DAYS_LEAP : __MONTH_DAYS_REGULAR, date.tm_mon - 1), 3)
                    }, "%m": function (date) {
                        return leadingNulls(date.tm_mon + 1, 2)
                    }, "%M": function (date) {
                        return leadingNulls(date.tm_min, 2)
                    }, "%n": function () {
                        return "\n"
                    }, "%p": function (date) {
                        if (date.tm_hour >= 0 && date.tm_hour < 12) {
                            return "AM"
                        } else {
                            return "PM"
                        }
                    }, "%S": function (date) {
                        return leadingNulls(date.tm_sec, 2)
                    }, "%t": function () {
                        return "\t"
                    }, "%u": function (date) {
                        return date.tm_wday || 7
                    }, "%U": function (date) {
                        var janFirst = new Date(date.tm_year + 1900, 0, 1);
                        var firstSunday = janFirst.getDay() === 0 ? janFirst : __addDays(janFirst, 7 - janFirst.getDay());
                        var endDate = new Date(date.tm_year + 1900, date.tm_mon, date.tm_mday);
                        if (compareByDay(firstSunday, endDate) < 0) {
                            var februaryFirstUntilEndMonth = __arraySum(__isLeapYear(endDate.getFullYear()) ? __MONTH_DAYS_LEAP : __MONTH_DAYS_REGULAR, endDate.getMonth() - 1) - 31;
                            var firstSundayUntilEndJanuary = 31 - firstSunday.getDate();
                            var days = firstSundayUntilEndJanuary + februaryFirstUntilEndMonth + endDate.getDate();
                            return leadingNulls(Math.ceil(days / 7), 2)
                        }
                        return compareByDay(firstSunday, janFirst) === 0 ? "01" : "00"
                    }, "%V": function (date) {
                        var janFourthThisYear = new Date(date.tm_year + 1900, 0, 4);
                        var janFourthNextYear = new Date(date.tm_year + 1901, 0, 4);
                        var firstWeekStartThisYear = getFirstWeekStartDate(janFourthThisYear);
                        var firstWeekStartNextYear = getFirstWeekStartDate(janFourthNextYear);
                        var endDate = __addDays(new Date(date.tm_year + 1900, 0, 1), date.tm_yday);
                        if (compareByDay(endDate, firstWeekStartThisYear) < 0) {
                            return "53"
                        }
                        if (compareByDay(firstWeekStartNextYear, endDate) <= 0) {
                            return "01"
                        }
                        var daysDifference;
                        if (firstWeekStartThisYear.getFullYear() < date.tm_year + 1900) {
                            daysDifference = date.tm_yday + 32 - firstWeekStartThisYear.getDate()
                        } else {
                            daysDifference = date.tm_yday + 1 - firstWeekStartThisYear.getDate()
                        }
                        return leadingNulls(Math.ceil(daysDifference / 7), 2)
                    }, "%w": function (date) {
                        return date.tm_wday
                    }, "%W": function (date) {
                        var janFirst = new Date(date.tm_year, 0, 1);
                        var firstMonday = janFirst.getDay() === 1 ? janFirst : __addDays(janFirst, janFirst.getDay() === 0 ? 1 : 7 - janFirst.getDay() + 1);
                        var endDate = new Date(date.tm_year + 1900, date.tm_mon, date.tm_mday);
                        if (compareByDay(firstMonday, endDate) < 0) {
                            var februaryFirstUntilEndMonth = __arraySum(__isLeapYear(endDate.getFullYear()) ? __MONTH_DAYS_LEAP : __MONTH_DAYS_REGULAR, endDate.getMonth() - 1) - 31;
                            var firstMondayUntilEndJanuary = 31 - firstMonday.getDate();
                            var days = firstMondayUntilEndJanuary + februaryFirstUntilEndMonth + endDate.getDate();
                            return leadingNulls(Math.ceil(days / 7), 2)
                        }
                        return compareByDay(firstMonday, janFirst) === 0 ? "01" : "00"
                    }, "%y": function (date) {
                        return (date.tm_year + 1900).toString().substring(2)
                    }, "%Y": function (date) {
                        return date.tm_year + 1900
                    }, "%z": function (date) {
                        var off = date.tm_gmtoff;
                        var ahead = off >= 0;
                        off = Math.abs(off) / 60;
                        off = off / 60 * 100 + off % 60;
                        return (ahead ? "+" : "-") + String("0000" + off).slice(-4)
                    }, "%Z": function (date) {
                        return date.tm_zone
                    }, "%%": function () {
                        return "%"
                    }
                };
                for (var rule in EXPANSION_RULES_2) {
                    if (pattern.includes(rule)) {
                        pattern = pattern.replace(new RegExp(rule, "g"), EXPANSION_RULES_2[rule](date))
                    }
                }
                var bytes = intArrayFromString(pattern, false);
                if (bytes.length > maxsize) {
                    return 0
                }
                writeArrayToMemory(bytes, s);
                return bytes.length - 1
            }

            function _strftime_l(s, maxsize, format, tm) {
                return _strftime(s, maxsize, format, tm)
            }

            var FSNode = function (parent, name, mode, rdev) {
                if (!parent) {
                    parent = this
                }
                this.parent = parent;
                this.mount = parent.mount;
                this.mounted = null;
                this.id = FS.nextInode++;
                this.name = name;
                this.mode = mode;
                this.node_ops = {};
                this.stream_ops = {};
                this.rdev = rdev
            };
            var readMode = 292 | 73;
            var writeMode = 146;
            Object.defineProperties(FSNode.prototype, {
                read: {
                    get: function () {
                        return (this.mode & readMode) === readMode
                    }, set: function (val) {
                        val ? this.mode |= readMode : this.mode &= ~readMode
                    }
                }, write: {
                    get: function () {
                        return (this.mode & writeMode) === writeMode
                    }, set: function (val) {
                        val ? this.mode |= writeMode : this.mode &= ~writeMode
                    }
                }, isFolder: {
                    get: function () {
                        return FS.isDir(this.mode)
                    }
                }, isDevice: {
                    get: function () {
                        return FS.isChrdev(this.mode)
                    }
                }
            });
            FS.FSNode = FSNode;
            FS.staticInit();
            Module["FS_createPath"] = FS.createPath;
            Module["FS_createDataFile"] = FS.createDataFile;
            Module["FS_createPreloadedFile"] = FS.createPreloadedFile;
            Module["FS_createLazyFile"] = FS.createLazyFile;
            Module["FS_createDevice"] = FS.createDevice;
            Module["FS_unlink"] = FS.unlink;
            InternalError = Module["InternalError"] = extendError(Error, "InternalError");
            embind_init_charCodes();
            BindingError = Module["BindingError"] = extendError(Error, "BindingError");
            init_ClassHandle();
            init_RegisteredPointer();
            init_embind();
            UnboundTypeError = Module["UnboundTypeError"] = extendError(Error, "UnboundTypeError");
            init_emval();

            function intArrayFromString(stringy, dontAddNull, length) {
                var len = length > 0 ? length : lengthBytesUTF8(stringy) + 1;
                var u8array = new Array(len);
                var numBytesWritten = stringToUTF8Array(stringy, u8array, 0, u8array.length);
                if (dontAddNull) u8array.length = numBytesWritten;
                return u8array
            }

            var asmLibraryArg = {
                "A": ___assert_fail,
                "n": ___cxa_allocate_exception,
                "p": ___cxa_begin_catch,
                "z": ___cxa_end_catch,
                "b": ___cxa_find_matching_catch_2,
                "i": ___cxa_find_matching_catch_3,
                "Ka": ___cxa_find_matching_catch_4,
                "o": ___cxa_free_exception,
                "ca": ___cxa_rethrow,
                "J": ___cxa_throw,
                "xa": ___cxa_uncaught_exceptions,
                "d": ___resumeException,
                "fa": ___sys_fcntl64,
                "Ea": ___sys_getdents64,
                "Ca": ___sys_ioctl,
                "ha": ___sys_open,
                "Da": ___sys_stat64,
                "Qa": __embind_finalize_value_array,
                "Va": __embind_register_bigint,
                "Ga": __embind_register_bool,
                "u": __embind_register_class,
                "x": __embind_register_class_class_function,
                "r": __embind_register_class_class_property,
                "w": __embind_register_class_constructor,
                "h": __embind_register_class_function,
                "G": __embind_register_class_property,
                "R": __embind_register_constant,
                "Fa": __embind_register_emval,
                "Q": __embind_register_enum,
                "P": __embind_register_enum_value,
                "ia": __embind_register_float,
                "E": __embind_register_function,
                "L": __embind_register_integer,
                "K": __embind_register_memory_view,
                "t": __embind_register_smart_ptr,
                "ja": __embind_register_std_string,
                "_": __embind_register_std_wstring,
                "pa": __embind_register_value_array,
                "Y": __embind_register_value_array_element,
                "Ha": __embind_register_void,
                "O": __emval_as,
                "Ra": __emval_call,
                "X": __emval_call_void_method,
                "ga": __emval_decref,
                "W": __emval_get_method_caller,
                "Pa": __emval_get_property,
                "ka": __emval_incref,
                "Ta": __emval_new_array,
                "Oa": __emval_new_cstring,
                "La": __emval_new_object,
                "Na": __emval_run_destructors,
                "Sa": __emval_set_property,
                "B": __emval_take_value,
                "Ma": __emval_typeof,
                "ea": _abort,
                "ba": _emscripten_asm_const_int,
                "va": _emscripten_memcpy_big,
                "Z": _emscripten_resize_heap,
                "ya": _environ_get,
                "za": _environ_sizes_get,
                "U": _fd_close,
                "Ba": _fd_read,
                "Ua": _fd_seek,
                "Aa": _fd_write,
                "a": _getTempRet0,
                "Ia": invoke_di,
                "aa": invoke_fi,
                "oa": invoke_fifii,
                "sa": invoke_fii,
                "ua": invoke_fiii,
                "v": invoke_i,
                "g": invoke_ii,
                "Ja": invoke_iid,
                "na": invoke_iif,
                "$": invoke_iiff,
                "V": invoke_iifff,
                "ma": invoke_iiffff,
                "e": invoke_iii,
                "j": invoke_iiii,
                "s": invoke_iiiii,
                "da": invoke_iiiiid,
                "H": invoke_iiiiii,
                "C": invoke_iiiiiii,
                "N": invoke_iiiiiiii,
                "T": invoke_iiiiiiiiiiii,
                "l": invoke_v,
                "m": invoke_vi,
                "ta": invoke_vid,
                "I": invoke_vif,
                "f": invoke_vii,
                "qa": invoke_viif,
                "c": invoke_viii,
                "k": invoke_viiii,
                "la": invoke_viiiiffiiii,
                "q": invoke_viiiii,
                "y": invoke_viiiiii,
                "F": invoke_viiiiiii,
                "ra": invoke_viiiiiiiii,
                "M": invoke_viiiiiiiiii,
                "S": invoke_viiiiiiiiiiiiiii,
                "D": _llvm_eh_typeid_for,
                "Wa": _setTempRet0,
                "wa": _strftime_l
            };
            var asm = createWasm();
            var ___wasm_call_ctors = Module["___wasm_call_ctors"] = function () {
                return (___wasm_call_ctors = Module["___wasm_call_ctors"] = Module["asm"]["Ya"]).apply(null, arguments)
            };
            var _malloc = Module["_malloc"] = function () {
                return (_malloc = Module["_malloc"] = Module["asm"]["Za"]).apply(null, arguments)
            };
            var ___errno_location = Module["___errno_location"] = function () {
                return (___errno_location = Module["___errno_location"] = Module["asm"]["$a"]).apply(null, arguments)
            };
            var _free = Module["_free"] = function () {
                return (_free = Module["_free"] = Module["asm"]["ab"]).apply(null, arguments)
            };
            var ___getTypeName = Module["___getTypeName"] = function () {
                return (___getTypeName = Module["___getTypeName"] = Module["asm"]["bb"]).apply(null, arguments)
            };
            var ___embind_register_native_and_builtin_types = Module["___embind_register_native_and_builtin_types"] = function () {
                return (___embind_register_native_and_builtin_types = Module["___embind_register_native_and_builtin_types"] = Module["asm"]["cb"]).apply(null, arguments)
            };
            var stackSave = Module["stackSave"] = function () {
                return (stackSave = Module["stackSave"] = Module["asm"]["db"]).apply(null, arguments)
            };
            var stackRestore = Module["stackRestore"] = function () {
                return (stackRestore = Module["stackRestore"] = Module["asm"]["eb"]).apply(null, arguments)
            };
            var _setThrew = Module["_setThrew"] = function () {
                return (_setThrew = Module["_setThrew"] = Module["asm"]["fb"]).apply(null, arguments)
            };
            var ___cxa_can_catch = Module["___cxa_can_catch"] = function () {
                return (___cxa_can_catch = Module["___cxa_can_catch"] = Module["asm"]["gb"]).apply(null, arguments)
            };
            var ___cxa_is_pointer_type = Module["___cxa_is_pointer_type"] = function () {
                return (___cxa_is_pointer_type = Module["___cxa_is_pointer_type"] = Module["asm"]["hb"]).apply(null, arguments)
            };
            var dynCall_jiji = Module["dynCall_jiji"] = function () {
                return (dynCall_jiji = Module["dynCall_jiji"] = Module["asm"]["ib"]).apply(null, arguments)
            };
            var dynCall_viijii = Module["dynCall_viijii"] = function () {
                return (dynCall_viijii = Module["dynCall_viijii"] = Module["asm"]["jb"]).apply(null, arguments)
            };
            var dynCall_iiiiij = Module["dynCall_iiiiij"] = function () {
                return (dynCall_iiiiij = Module["dynCall_iiiiij"] = Module["asm"]["kb"]).apply(null, arguments)
            };
            var dynCall_iiiiijj = Module["dynCall_iiiiijj"] = function () {
                return (dynCall_iiiiijj = Module["dynCall_iiiiijj"] = Module["asm"]["lb"]).apply(null, arguments)
            };
            var dynCall_iiiiiijj = Module["dynCall_iiiiiijj"] = function () {
                return (dynCall_iiiiiijj = Module["dynCall_iiiiiijj"] = Module["asm"]["mb"]).apply(null, arguments)
            };

            function invoke_ii(index, a1) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viii(index, a1, a2, a3) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_vii(index, a1, a2) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiiii(index, a1, a2, a3, a4) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iii(index, a1, a2) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiii(index, a1, a2, a3, a4) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiii(index, a1, a2, a3) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiii(index, a1, a2, a3, a4, a5) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiiiii(index, a1, a2, a3, a4, a5) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4, a5)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_vi(index, a1) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_i(index) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)()
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiiii(index, a1, a2, a3, a4, a5, a6) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5, a6)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_v(index) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)()
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_vif(index, a1, a2) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_fifii(index, a1, a2, a3, a4) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiiiiii(index, a1, a2, a3, a4, a5, a6) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4, a5, a6)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iif(index, a1, a2) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iid(index, a1, a2) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_fi(index, a1) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_di(index, a1) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiiiiiiii(index, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iifff(index, a1, a2, a3, a4) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiffff(index, a1, a2, a3, a4, a5) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4, a5)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiiiiiii(index, a1, a2, a3, a4, a5, a6, a7) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiiiii(index, a1, a2, a3, a4, a5, a6, a7) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiff(index, a1, a2, a3) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiiffiiii(index, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_fiii(index, a1, a2, a3) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_vid(index, a1, a2) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_fii(index, a1, a2) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiiiiiii(index, a1, a2, a3, a4, a5, a6, a7, a8, a9) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7, a8, a9)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiiiid(index, a1, a2, a3, a4, a5) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4, a5)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_iiiiiiiiiiii(index, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) {
                var sp = stackSave();
                try {
                    return wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viiiiiiiiiiiiiii(index, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            function invoke_viif(index, a1, a2, a3) {
                var sp = stackSave();
                try {
                    wasmTable.get(index)(a1, a2, a3)
                } catch (e) {
                    stackRestore(sp);
                    if (e !== e + 0 && e !== "longjmp") throw e;
                    _setThrew(1, 0)
                }
            }

            Module["addRunDependency"] = addRunDependency;
            Module["removeRunDependency"] = removeRunDependency;
            Module["FS_createPath"] = FS.createPath;
            Module["FS_createDataFile"] = FS.createDataFile;
            Module["FS_createPreloadedFile"] = FS.createPreloadedFile;
            Module["FS_createLazyFile"] = FS.createLazyFile;
            Module["FS_createDevice"] = FS.createDevice;
            Module["FS_unlink"] = FS.unlink;
            var calledRun;

            function ExitStatus(status) {
                this.name = "ExitStatus";
                this.message = "Program terminated with exit(" + status + ")";
                this.status = status
            }

            dependenciesFulfilled = function runCaller() {
                if (!calledRun) run();
                if (!calledRun) dependenciesFulfilled = runCaller
            };

            function run(args) {
                args = args || arguments_;
                if (runDependencies > 0) {
                    return
                }
                preRun();
                if (runDependencies > 0) {
                    return
                }

                function doRun() {
                    if (calledRun) return;
                    calledRun = true;
                    Module["calledRun"] = true;
                    if (ABORT) return;
                    initRuntime();
                    readyPromiseResolve(Module);
                    if (Module["onRuntimeInitialized"]) Module["onRuntimeInitialized"]();
                    postRun()
                }

                if (Module["setStatus"]) {
                    Module["setStatus"]("Running...");
                    setTimeout(function () {
                        setTimeout(function () {
                            Module["setStatus"]("")
                        }, 1);
                        doRun()
                    }, 1)
                } else {
                    doRun()
                }
            }

            Module["run"] = run;
            if (Module["preInit"]) {
                if (typeof Module["preInit"] == "function") Module["preInit"] = [Module["preInit"]];
                while (Module["preInit"].length > 0) {
                    Module["preInit"].pop()()
                }
            }
            run();
            var postRegistrations = [];

            function onModuleReady(callback) {
                postRegistrations.push(callback)
            }

            Module.onRuntimeInitialized = function () {
                for (var callback of postRegistrations) {
                    callback()
                }
            };
            (function () {
                var nodeFs;
                var nodePath;
                var nodeProcess;
                var pathSep;
                var wasmPathSep = "/";
                var ENVIRONMENT_IS_WEB;
                var ENVIRONMENT_IS_NODE;
                var PATH_LIST_SEPARATOR = ";";
                var callId = 0;
                var MAX_CALL_ID = 99999;

                function removeDuplicates(array) {
                    var seen = {};
                    return array.filter(function (item) {
                        return seen.hasOwnProperty(item) ? false : seen[item] = true
                    })
                }

                function createFilePath(fileName, filePath, sep = pathSep) {
                    var pathSlash = filePath.endsWith(sep);
                    var fileSlash = fileName.startsWith(sep);
                    var path;
                    if (pathSlash || fileSlash) {
                        if (pathSlash && fileSlash) {
                            path = filePath.substring(0, filePath.length - 1) + fileName
                        } else {
                            path = filePath + fileName
                        }
                    } else {
                        path = filePath + sep + fileName
                    }
                    return path
                }

                function fetchXml(fileName, searchPaths) {
                    var i = 0;

                    function fetchHandler() {
                        var filePath = createFilePath(fileName, searchPaths[i++]);
                        return fetch(filePath).then(function (response) {
                            if (response.status === 200) {
                                return response.text().then(function (data) {
                                    var url = new URL(response.url);
                                    var filePath = url.pathname.substring(1);
                                    filePath = filePath.replace(new RegExp(pathSep, "g"), wasmPathSep);
                                    return {data: data, filePath: filePath, fullPath: url.origin + url.pathname}
                                })
                            } else if (i < searchPaths.length) {
                                return fetchHandler()
                            } else {
                                throw new Error("MaterialX file not found: " + fileName)
                            }
                        })
                    }

                    return fetchHandler()
                }

                function loadXml(fileName, searchPaths) {
                    var i = 0;

                    function loadHandler() {
                        var filePath = createFilePath(fileName, searchPaths[i++]);
                        filePath = nodePath.resolve(filePath);
                        return new Promise(function (resolve, reject) {
                            nodeFs.readFile(filePath, "utf8", function (err, data) {
                                if (err) {
                                    if (i < searchPaths.length) {
                                        resolve(loadHandler())
                                    } else {
                                        reject(new Error("MaterialX file not found: " + fileName))
                                    }
                                } else {
                                    var parsedPath = nodePath.parse(filePath);
                                    var path = filePath.substring(parsedPath.root.length);
                                    var sep = pathSep === "\\" ? "\\\\" : pathSep;
                                    path = path.replace(new RegExp(sep, "g"), wasmPathSep);
                                    resolve({data: data, filePath: path, fullPath: filePath})
                                }
                            })
                        })
                    }

                    return loadHandler()
                }

                function prepareSearchPaths(searchPath) {
                    var newSearchPath = searchPath.concat(PATH_LIST_SEPARATOR, Module.getEnviron(Module.MATERIALX_SEARCH_PATH_ENV_VAR));
                    var searchPaths = ["." + pathSep].concat(newSearchPath.split(PATH_LIST_SEPARATOR));
                    var i = searchPaths.length - 1;
                    while (i) {
                        if (searchPaths[i].trim() === "") {
                            searchPaths.splice(i, 1)
                        }
                        --i
                    }
                    return removeDuplicates(searchPaths)
                }

                function makeWasmAbsolute(paths, wasmRootFolder) {
                    var pathList = paths;
                    if (typeof paths === "string") {
                        pathList = paths.split(PATH_LIST_SEPARATOR)
                    }
                    for (var i = 0; i < pathList.length; ++i) {
                        var path = pathList[i];
                        if (ENVIRONMENT_IS_NODE) {
                            if (nodePath.isAbsolute(path)) {
                                var parsed = nodePath.parse(path);
                                path = wasmRootFolder + wasmPathSep + parsed.dir.substring(parsed.root.length)
                            }
                            var sep = pathSep === "\\" ? "\\\\" : pathSep;
                            path.replace(new RegExp(sep, "g"), wasmPathSep)
                        } else if (ENVIRONMENT_IS_WEB) {
                            var link = document.createElement("a");
                            link.href = path;
                            if (link.origin + link.pathname + link.search + link.hash === path) {
                                path = wasmRootFolder + link.pathname
                            }
                        } else {
                            throw new Error("Unknown environment!")
                        }
                        pathList[i] = path
                    }
                    if (typeof paths === "string") {
                        return pathList.join(Module.PATH_LIST_SEPARATOR)
                    } else {
                        return pathList
                    }
                }

                function getWasmSearchPath(searchPath, wasmRootFolder) {
                    var wasmSearchPath = searchPath.split(PATH_LIST_SEPARATOR);
                    makeWasmAbsolute(wasmSearchPath, wasmRootFolder);
                    wasmSearchPath.push(getWasmCwd(wasmRootFolder));
                    wasmSearchPath = removeDuplicates(wasmSearchPath);
                    wasmSearchPath = wasmSearchPath.join(Module.PATH_LIST_SEPARATOR);
                    return wasmSearchPath
                }

                function getIncludes(file) {
                    var includeRegex = /<xi:include href="(.*)"\s*\/>/g;
                    var matches = file.matchAll(includeRegex);
                    var includes = [];
                    for (var match of matches) {
                        includes.push(match[1])
                    }
                    return includes
                }

                function loadFile(fileToLoad, searchPaths) {
                    var promise;
                    if (ENVIRONMENT_IS_WEB) {
                        promise = fetchXml(fileToLoad, searchPaths)
                    } else if (ENVIRONMENT_IS_NODE) {
                        promise = loadXml(fileToLoad, searchPaths)
                    } else {
                        throw new Error("Unknown environment!")
                    }
                    return promise
                }

                function trackPath(path, filesUploaded, isFile = false) {
                    if (isFile) {
                        if (!filesUploaded.files) {
                            filesUploaded.files = []
                        }
                        filesUploaded.files.push(path)
                    } else {
                        if (!filesUploaded.folders) {
                            filesUploaded.folders = []
                        }
                        filesUploaded.folders.splice(0, 0, path)
                    }
                }

                function createInWasm(file, data, filesUploaded, wasmRootFolder, isFile = true) {
                    var folders;
                    if (isFile) {
                        folders = file.substring(1, file.lastIndexOf(wasmPathSep)).split(wasmPathSep)
                    } else {
                        folders = file.substring(wasmRootFolder.length).split(wasmPathSep)
                    }
                    var folder = wasmRootFolder;
                    for (var i = 1; i < folders.length; ++i) {
                        folder += wasmPathSep + folders[i];
                        var dirExists;
                        try {
                            var stat = FS.stat(folder);
                            dirExists = FS.isDir(stat.mode)
                        } catch (e) {
                            dirExists = false
                        }
                        if (!dirExists) {
                            try {
                                FS.mkdir(folder);
                                trackPath(folder, filesUploaded)
                            } catch (e) {
                                throw new Error("Failed to create folder in WASM FS.")
                            }
                        }
                    }
                    if (isFile) {
                        try {
                            FS.writeFile(file, data);
                            trackPath(file, filesUploaded, true)
                        } catch (e) {
                            throw new Error("Failed to store file in WASM FS.")
                        }
                    }
                }

                function getWasmCwd(wasmRootFolder) {
                    if (ENVIRONMENT_IS_NODE) {
                        var cwd = nodeProcess.cwd();
                        var parsed = nodePath.parse(cwd);
                        var wasmCwd = wasmRootFolder + wasmPathSep + cwd.substring(parsed.root.length);
                        var sep = pathSep === "\\" ? "\\\\" : pathSep;
                        return wasmCwd.replace(new RegExp(sep, "g"), wasmPathSep)
                    } else if (ENVIRONMENT_IS_WEB) {
                        var cwd = window.location.pathname;
                        cwd = cwd.substring(0, cwd.lastIndexOf(pathSep));
                        return createFilePath(cwd, wasmRootFolder, wasmPathSep)
                    } else {
                        throw new Error("Unknown environment!")
                    }
                }

                function storeFileToDisk(fileName, content) {
                    if (ENVIRONMENT_IS_NODE) {
                        try {
                            nodeFs.writeFileSync(fileName, content)
                        } catch (e) {
                            throw new Error("Failed to write file '" + fileName + "': " + e.message)
                        }
                    } else if (ENVIRONMENT_IS_WEB) {
                        var pos = fileName.lastIndexOf(pathSep);
                        fileName = fileName.substring(pos > -1 ? pos + 1 : 0);
                        var element = document.createElement("a");
                        element.setAttribute("href", "data:text/plain;charset=utf-8," + encodeURIComponent(content));
                        element.setAttribute("download", fileName);
                        element.style.display = "none";
                        document.body.appendChild(element);
                        element.click();
                        document.body.removeChild(element)
                    }
                }

                onModuleReady(function () {
                    ENVIRONMENT_IS_WEB = typeof window === "object";
                    ENVIRONMENT_IS_NODE = typeof process === "object" && typeof process.versions === "object" && typeof process.versions.node === "string";
                    if (ENVIRONMENT_IS_WEB) {
                        pathSep = "/"
                    }
                    if (ENVIRONMENT_IS_NODE) {
                        nodeFs = require("fs");
                        nodePath = require("path");
                        nodeProcess = require("process");
                        pathSep = nodePath.sep
                    }

                    function _readFromXmlString(doc, str, searchPath, readOptions, filesLoaded = [], initialFilePath = "") {
                        var wasmRootFolder = "/readFromXml" + callId++ % MAX_CALL_ID;
                        var searchPaths = prepareSearchPaths(searchPath);
                        try {
                            FS.mkdir(wasmRootFolder)
                        } catch (e) {
                            throw new Error("Failed to create folder in WASM FS.")
                        }
                        var includes = [];
                        if (!readOptions || readOptions.readXIncludes) {
                            includes = getIncludes(str)
                        }
                        var filesUploaded = {files: [], folders: []};
                        var wasmCwd = getWasmCwd(wasmRootFolder);
                        var initialFileName = wasmCwd + "/ChosenToHopefullyNotClashWithAnyOtherFile123";
                        if (initialFilePath) {
                            var sep = pathSep === "\\" ? "\\\\" : pathSep;
                            initialFileName = initialFilePath.replace(new RegExp(sep, "g"), wasmPathSep);
                            initialFileName = createFilePath(initialFileName, wasmRootFolder, wasmPathSep);
                            createInWasm(wasmCwd, null, filesUploaded, wasmRootFolder, false)
                        }
                        createInWasm(initialFileName, str, filesUploaded, wasmRootFolder);

                        function loadFiles(filesLoadedList, fileList, pathsList) {
                            var promises = [Promise.resolve()];
                            for (var fileToLoad of fileList) {
                                var filesLoadedCopy = filesLoadedList.slice();
                                var searchPathsCopy = pathsList.slice();
                                var promise = loadFile(fileToLoad, searchPathsCopy).then(function (result) {
                                    if (filesLoadedCopy.includes(result.fullPath)) {
                                        throw new Error("Cycle detected!\n" + filesLoadedCopy.join("\n-> ") + "\n-> " + result.fullPath)
                                    }
                                    filesLoadedCopy.push(result.fullPath);
                                    var pos = result.fullPath.lastIndexOf(pathSep);
                                    var path = result.fullPath.substring(0, pos > -1 ? pos : 0);
                                    if (!searchPathsCopy.includes(path)) {
                                        searchPathsCopy.splice(0, 0, path)
                                    }
                                    var includes = getIncludes(result.data);
                                    var wasmPath = createFilePath(result.filePath, wasmRootFolder, wasmPathSep);
                                    if (!filesUploaded.files.includes(wasmPath)) {
                                        createInWasm(wasmPath, result.data, filesUploaded, wasmRootFolder)
                                    }
                                    return loadFiles(filesLoadedCopy, includes, searchPathsCopy)
                                });
                                promises.push(promise)
                            }
                            return Promise.all(promises)
                        }

                        return loadFiles(filesLoaded, includes, searchPaths).then(function () {
                            var wasmSearchPath = getWasmSearchPath(searchPath, wasmRootFolder);
                            FS.chdir(wasmCwd);
                            try {
                                var searchPathEnv = Module.getEnviron(Module.MATERIALX_SEARCH_PATH_ENV_VAR);
                                if (searchPathEnv) {
                                    var wasmSearchPathEnv = makeWasmAbsolute(searchPathEnv, wasmRootFolder);
                                    Module.setEnviron(Module.MATERIALX_SEARCH_PATH_ENV_VAR, wasmSearchPathEnv)
                                }
                                Module._readFromXmlFile(doc, initialFileName, wasmSearchPath, readOptions);
                                if (searchPathEnv) {
                                    Module.setEnviron(Module.MATERIALX_SEARCH_PATH_ENV_VAR, searchPathEnv)
                                }
                            } catch (errPtr) {
                                throw new Error("Failed to read MaterialX files from WASM FS: " + Module.getExceptionMessage(errPtr))
                            }
                            try {
                                for (var file of filesUploaded.files) {
                                    FS.unlink(file)
                                }
                                FS.chdir("/");
                                for (var folder of filesUploaded.folders) {
                                    FS.rmdir(folder)
                                }
                                FS.rmdir(wasmRootFolder)
                            } catch (e) {
                                throw new Error("Failed to delete temporary files from WASM FS.")
                            }
                        })
                    }

                    Module.readFromXmlString = function (doc, str, searchPath = "", readOptions = null) {
                        if (arguments.length < 2 || arguments.length > 4) {
                            throw new Error("Function readFromXmlString called with an invalid number of arguments (" + arguments.length + ") - expects 2 to 4!")
                        }
                        return _readFromXmlString(doc, str, searchPath, readOptions)
                    };
                    Module.readFromXmlFile = function (doc, fileName, searchPath = "", readOptions = null) {
                        if (arguments.length < 2 || arguments.length > 4) {
                            throw new Error("Function readFromXmlFile called with an invalid number of arguments (" + arguments.length + ") - expects 2 to 4!")
                        }
                        var searchPaths = prepareSearchPaths(searchPath);
                        return loadFile(fileName, searchPaths).then(function (result) {
                            var filesLoaded = [result.fullPath];
                            var pos = result.fullPath.lastIndexOf(pathSep);
                            var path = result.fullPath.substring(0, pos > -1 ? pos : 0);
                            searchPath = searchPath.concat(PATH_LIST_SEPARATOR, path);
                            return _readFromXmlString(doc, result.data, searchPath, readOptions, filesLoaded, result.filePath)
                        })
                    };
                    Module.writeToXmlFile = function (doc, fileName, writeOptions = null) {
                        if (arguments.length < 2 || arguments.length > 3) {
                            throw new Error("Function writeToXmlFile called with an invalid number of arguments (" + arguments.length + ") - expects 2 to 3!")
                        }
                        var file = Module.writeToXmlString(doc, writeOptions);
                        storeFileToDisk(fileName, file)
                    };
                    Module.exportToXmlFile = function (doc, fileName, exportOptions = null) {
                        if (arguments.length < 2 || arguments.length > 3) {
                            throw new Error("Function exportToXmlFile called with an invalid number of arguments (" + arguments.length + ") - expects 2 to 3!")
                        }
                        var file = Module.exportToXmlString(doc, exportOptions);
                        storeFileToDisk(fileName, file)
                    }
                })
            })();


            return MaterialX.ready
        }
    );
})();
if (typeof exports === 'object' && typeof module === 'object')
    module.exports = MaterialX;
else if (typeof define === 'function' && define['amd'])
    define([], function () {
        return MaterialX;
    });
else if (typeof exports === 'object')
    exports["MaterialX"] = MaterialX;
