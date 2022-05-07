[ $# != 0 ] && {
  cp ~/H8/src/cog/tmp/.output/Products/Release/cog .
  ruby codeSignCLine.rb -app="cog" -devid="Developer ID Application: Brian Hapgood (HE96RQ5ZY9)" -kcpass="$1"
} || {
  echo "Usage: sign-cog.sh <password>"
}
