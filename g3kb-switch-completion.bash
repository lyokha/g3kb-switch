#!/usr/bin/env bash

_g3kb_switch_completion()
{
    if [ "${#COMP_WORDS[@]}" -gt 3 ] ; then
        return
    fi

    local comp_opt="-h -p -l -n -s"

    if [ "${#COMP_WORDS[@]}" -eq 3 ] ; then
        if [ "${COMP_WORDS[1]}" = "-s" ] ; then
            local IFS=$'\n'
            COMPREPLY=( $(printf "%q\n" \
                $(compgen -W "$(g3kb-switch -l)" \
                    -- "$(printf "%q\n" "${COMP_WORDS[2]}")")) )
        fi
    else
        local IFS=$' \t\n'
        COMPREPLY=( $(compgen -W "$comp_opt" -- "${COMP_WORDS[1]}") )
    fi
}

complete -F _g3kb_switch_completion -o nosort g3kb-switch

