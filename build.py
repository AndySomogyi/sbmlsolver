import os, sys
import requests
import zipfile

def download_url(url, save_path, chunk_size=128):
    r = requests.get(url, stream=True)
    with open(save_path, 'wb') as fd:
        for chunk in r.iter_content(chunk_size=chunk_size):
            fd.write(chunk)

if sys.platform == "win32":
    # args
    msvc_root = "C:\Program Files (x86)\Microsoft Visual Studio",
    msvc_version = "2019",
    build_config = "Release",
    verbose = "Verbose"
    pause = "Pause"

    if not os.path.isdir(msvc_root):
        raise NotADirectoryError("Path given as msvc_root is not a real directory")

    msvc_version_root = os.path.join(msvc_root, msvc_version)
    if not os.path.isfile(msvc_version_root):
        raise NotADirectoryError("Requested Visual Studio version has not been found.")

    vcvars64 = os.path.join(msvc_version_root, r"Community\VC\Auxiliary\Build\vcvars64.bat")
    if not os.path.isfile(vcvars64):
        raise FileExistsError("vcvars64.bat not found")

    configurations = ['Release', "Debug"]
    if build_config not in configurations:
        raise ValueError(f"Unexpected configuration \"{build_config}\"given. Expected either Release or Debug.")

    llvm_zip = os.path.join(os.path.dirname(__file__), "LLVM6.0.1.zip")
    llvm_dir = os.path.join(os.path.dirname(__file__), "LLVM6.0.1")
    llvm_url = f"https://github.com/sys-bio/LLVM6.0.1/releases/download/v0.0.1/LLVM6.0.1-windows10-msvc2019.zip"

    if not (os.path.isfile(llvm_zip)):
        download_url(llvm_url, llvm_zip)

    if not os.path.isfile(llvm_zip):
        raise ValueError("Could not download llvm")

    if not os.path.isdir(llvm_dir):
        with zipfile.ZipFile(llvm_zip, 'r') as zip_ref:
            zip_ref.extractall(llvm_dir)

    #todo some checks to verify llvm

    os.system("git clone https://github.com/CiaranWelsh/roadrunner.git ; cd roadrunner ; mkdir \"build-$build_config\" ; cd \"build-$build_config\"")

    developers_cmd_prompt = os.path.join(msvc_version_root, r"Community\Common7\Tools\VsDevCmd.bat")
    if not os.path.isfile(developers_cmd_prompt):
        raise ValueError("Developers command prompt not found")

    cmake_command = f"cmake -DLLVM_INSTALL_PREFIX=\"{llvm_dir}\" " \
                    f"-DCMAKE_INSTALL_PREFIX=\"..\install-windows\" " \
                    f"-DCMAKE_BUILD_TYPE=\"{build_config}\" -A x64 .. "
    make_command = f"msbuild /p:configuration={build_config} rr.sln "
    os.system(f"cmd /c {vcvars64} ; {cmake_command} ; {make_command}")











elif sys.platform == "linux":
    pass
elif sys.platform == "macos":
    raise ValueError("Mac builds not yet supported")
else:
    raise ValueError("Other systems not supported by the build script. ")




















