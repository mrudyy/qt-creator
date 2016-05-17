import qbs
import qbs.FileInfo

QtcProduct {
    type: ["application"]
    consoleApplication: true
    installDir:  qtc.ide_libexec_path

    cpp.rpaths: {
        var relativePathToLibs = FileInfo.relativePath('/' + qtc.ide_libexec_path,
                                                       '/' + qtc.ide_library_path);
        var relativePathToPlugins = FileInfo.relativePath('/' + qtc.ide_libexec_path,
                                                          '/' + qtc.ide_plugin_path);
        var prefix = qbs.targetOS.contains("osx") ? "@executable_path" : "$ORIGIN";
        return [
            FileInfo.joinPaths(prefix, relativePathToLibs),
            FileInfo.joinPaths(prefix, relativePathToPlugins)
        ];
    }
}
