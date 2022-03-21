#compdef g3kb-switch

_arguments \
  "(- *)"-v"[Show the program version and exit]" \
  "(- *)"-h"[Show this message and exit]" \
  "(- *)"-s"[Switch to layout group ARG]:(ARG):($(g3kb-switch -l))" \
  "(- *)"-n"[Switch to the next layout group]" \
  "(- *)"-l"[Show available layout groups]" \
  "(- *)"-p"[Show the current layout group (default)]" \
