#!/usr/bin/lua

if arg[2] == nil or arg[4] == nil then
  print ("Bad argument")
  return 0
end

pars = arg[2]
pars = string.sub(pars, 10, string.len(pars) - 5)
print ("[url="..arg[4].."]"..pars.."[/url]")

