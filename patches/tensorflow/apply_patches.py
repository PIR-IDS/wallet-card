from os.path import join, isfile

Import("env")

LIB_DIR = join(env.subst("$PROJECT_LIBDEPS_DIR"), env.subst("$PIOENV"))
TENSORFLOW_PROFILER_DIR = join(LIB_DIR, "Arduino_TensorFlowLite", "src", "tensorflow", "lite", "core", "api")

patchflag_path = join(TENSORFLOW_PROFILER_DIR, ".patching-done")

print("Patching Arduino_TensorFlowLite...")

# patch file only if we didn't do it before
if not isfile(join(TENSORFLOW_PROFILER_DIR, ".patching-done")):
    print("Patching profiler.h...")
    original_file = join(TENSORFLOW_PROFILER_DIR, "profiler.h")
    patched_file = join(env.subst("$PROJECT_DIR"), "patches", "tensorflow", "profiler.patch")

    assert isfile(original_file) and isfile(patched_file)

    try:
        import patch
    except ImportError:
        env.Execute("$PYTHONEXE -m pip install \"patch==1.*\"")
        import patch

    pset = patch.fromfile(patched_file)
    pset.apply(root=TENSORFLOW_PROFILER_DIR)


    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))

print("Patching done!")