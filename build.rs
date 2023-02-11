fn main() {
    #[cfg(target_os = "macos")]
    cc::Build::new()
        .file("./src/plattform/macos.c")
        .compile("native.a");
    #[cfg(target_os = "linux")]
    cc::Build::new()
        .file("./src/plattform/linux.c")
        .compile("native.a");
}
