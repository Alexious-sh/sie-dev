#!/usr/bin/lua

pars = arg[2]
pars = string.sub(pars, 10, string.len(pars) - 5)
print ("[url="..arg[4].."]"..pars.."[/url]")

