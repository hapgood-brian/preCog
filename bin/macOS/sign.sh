[ $# != 0 ] && {
  ruby codeSignCLine.rb -app="../../tmp/.output/Products/Release/player.app" -devid="Developer ID Application: Brian Hapgood (HE96RQ5ZY9)" -kcpass="$1"
} || {
  echo "Usage: sign.sh <password>"
}
