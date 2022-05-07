#------------------------------------------------------------------------------
# Copyright (c) Swordpoint Studios 2018-2020
#------------------------------------------------------------------------------
require_relative 'codeSign'
def process_args
  ARGV.each do |arg|
    commands = arg.split('=')
    if commands.length < 2
      break;
    end
    command = commands[0].delete('-').downcase
    value   = commands[1].delete("'").delete('"')
    case command
    when'app'
      $app_path = value
    when'devid'
      $dev_id = value #-dev=" -devid="Developer ID Application: Swordpoint Studios LLC (9MP9YWSP88)"
    when'kcpass'
      $pass = value
    else
      # do nothing
    end
  end
end

process_args()
result = sign_app($app_path,$dev_id,$pass)
unless result
  puts 'Code Sign Error'
  exit 1
end
result=verify_codesign($app_path)
unless result
  puts 'Could Not Verify App Was Signed Correctly '
  exit 1
end
