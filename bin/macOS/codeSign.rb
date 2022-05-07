#------------------------------------------------------------------------------
# Copyright (c) Swordpoint Studios 2018-2021
#------------------------------------------------------------------------------
require 'set'
require 'find'
EXCLUDE_EXT_SIGNING = Set["qml" , "icns", "qmltypes", "qmlc", "png", "jsc", "js", "svg", "ttf", "conf", "lproj","plist", "pak", "dat", "metainfo"]
EXCLUDE_FILES_SIGNING = Set["qmldir", "CodeResources", "pkginfo",".DS_Store"]

def sign_app(app_path,dev_id,password = nil)
  if password == nil
   keychain_password = ENV['PASSWORD']
  else
    keychain_password = password
  end
  #TODO pass in the password as argument instead of env var
  if keychain_password == nil || keychain_password.empty?
    raise "Could not find keychain password in PASSWORD env var"
  end
  files =  []
  Find.find("#{app_path}") do |file|
     ext = File.extname(file).strip.downcase[1..-1]
     file_name = File.basename(file, ".*").strip.downcase
     File.name
     unless (File.directory?(file) && ext != "framework" ) || EXCLUDE_EXT_SIGNING.include?(ext) || EXCLUDE_FILES_SIGNING.include?(file_name)
       files << file
     end
  end
  files.each do |file|
    #TODO parallelize this with waiting for all
    result = system("security unlock-keychain -p #{keychain_password} ~/Library/Keychains/login.keychain-db && codesign -f -o runtime -v --deep -s \"#{dev_id}\" \"#{file}\"")
    unless result
      puts "error signing #{file}"
    end
  end
  result = system("security unlock-keychain -p #{keychain_password} ~/Library/Keychains/login.keychain-db && codesign -f -o runtime -v -s \"#{dev_id}\" \"#{app_path}\"")
  unless result
    puts "Error Signing App #{app_path}"
  end
  return result
end

def verify_codesign(app_dir)
  puts "Verifying Code Signature"
  system("codesign --verify --verbose \"#{app_dir}\"")
end