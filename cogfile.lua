if platform.is'apple'then
  require'include/cogfile.xcode.lua'
elseif platform.is'microsoft'then
  require'include/cogfile.vs2019.lua'
elseif platform.is'linux'then
  require'include/cogfile.linux.lua'
end
