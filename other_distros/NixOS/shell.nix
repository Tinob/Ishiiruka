with import <nixpkgs> {};
stdenv.mkDerivation {

  name = "fasterProjectPlus";

  enableParallelBuilding = true;
  gtk2 = pkgs.gtk2;
  glib = pkgs.glib.out;

  nativeBuildInputs = [ pkgconfig cmake ];
  buildInputs = [ pkgconfig bluez ffmpeg libao libGLU_combined gtk2 gtk3 glib
  gettext xorg.libpthreadstubs xorg.libXrandr xorg.libXext xorg.libX11 xorg.libSM readline openal
  libevdev xorg.libXdmcp portaudio libusb libpulseaudio libudev gnumake wget
  wxGTK31 soundtouch miniupnpc mbedtls curl lzo sfml enet xdg_utils hidapi  ];

}
