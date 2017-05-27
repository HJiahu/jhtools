 -- Code based on https://github.com/cmderdev/cmder/issues/1056
 -- with modifications to make it work with conda/virtual envs (https://github.com/cmderdev/cmder/issues/1056#issuecomment-237403714)
---
 -- Find out current conda/virtual envs
 -- @return {false|conda/virtual env name}
---

local clink_path_lua_file = clink.get_env('CMDER_ROOT')..'\\vendor\\clink-completions\\modules\\path.lua'
dofile(clink_path_lua_file)

function get_virtual_env(env_var)
    env_path = clink.get_env(env_var)
    if env_path then
        basen = exports.basename(env_path)
        return basen
    end
    return false
end

---
 -- add conda env name 
---
function conda_prompt_filter()
    -- add in conda env name
    local conda_env = get_virtual_env('CONDA_DEFAULT_ENV')
    if conda_env then
        clink.prompt.value = string.gsub(clink.prompt.value, "λ", "["..conda_env.."] λ")
    end
end

---
 -- add virtual env name 
---
function venv_prompt_filter()
    -- add in virtual env name
    local venv = get_virtual_env('VIRTUAL_ENV')
    if venv then
        clink.prompt.value = string.gsub(clink.prompt.value, "λ", "["..venv.."] λ")
    end
end

clink.prompt.register_filter(conda_prompt_filter, 20)
clink.prompt.register_filter(venv_prompt_filter, 20)
