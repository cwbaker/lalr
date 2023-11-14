package.path = './model/?.lua;./ometa/?.lua;./lib/?.lua;./?.lua;' .. package.path

local utils = require 'utils'
local Types = require 'types'
local Any, Array = Types.Any, Types.Array

local OMeta = require 'ometa'
local Model = require 'ometa_abstractsyntax'
local Grammar = require 'ometa_lua_grammar'.OMetaInLuaGrammar

local alt = {'reference'}

local OMetaToLuaTranslator = require('ometa_ast2lua_ast_' .. alt[1])
local ToSourceTranslator = require 'lua_ast2source'

local function parseFile(path)
  return utils.measure(
    function() 
      return Grammar.block:matchFile(path)
    end
  )
end

local function translateTree(tree)
  return utils.measure(
    function() 
      return OMetaToLuaTranslator.toLuaAst(tree)
    end
  )
end

local function generateSourceTrans(tree)
  return utils.measure(
    function() 
      return ToSourceTranslator.trans:matchMixed(tree)
    end
  )
end

local function generateSourceDirect(tree)
  return utils.measure(
    function() 
      return tree:toLuaSource()
    end
  )
end

local function compileFile(name)
  print('----------------------------')
  print('compilation: ' .. name)
  local ometaAst, pt = parseFile(name)
  print(string.format('source2ast:  %.3f', pt))
  --utils.writeFile(name .. '.oast', tostring(ometaAst))
  local luaAst, tt = translateTree(ometaAst)
  print(string.format('ometa2lua:   %.3f', tt))
  --utils.writeFile(name .. '.last', tostring(luaAst))
  local luaSource, t2t = 
--                    generateSourceDirect(luaAst)
                    generateSourceTrans(luaAst)
  print(string.format('ast2source:  %.3f', t2t))
  utils.writeFile(name:sub(1, -5) .. '.lua', luaSource)
  return pt + tt + t2t
end

local libs = {'commons','grammar_commons','binary_commons','auxiliary','lua_grammar','lua52_grammar','ometa_grammar','ometa_lua_grammar','ometa_ast2lua_ast_' .. alt[1],'lua_ast2source'}

return {
  parseFile = parseFile,
  translateTree = translateTree,
  generateSource = generateSourceTrans,
  compileFile = compileFile;
  
  build = function() 
    local t = 0
    for l = 1, #libs do 
--      t = t + compileFile('./ometa/' .. libs[l])
      local pass, res = pcall(compileFile, './ometa/' .. libs[l] .. '.lpp')
      if not pass then
        print(res)
      else
        t = t + res
      end
    end
    print('----------------------------')
    print(string.format('total:       %.3f', t))
  end;
}
