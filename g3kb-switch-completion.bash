#!/usr/bin/env bash

_g3kb_switch_completion()
{
    if [ "${#COMP_WORDS[@]}" -gt 3 ] ; then
        return
    fi

    local comp_opt="-h -l -n -p -s"

    if [ "${#COMP_WORDS[@]}" -eq 3 ] ; then
        if [ "${COMP_WORDS[1]}" = "-s" ] ; then
            COMPREPLY=($(compgen -W "$(g3kb-switch -l)" -- "${COMP_WORDS[2]}"))
        fi
    else
        COMPREPLY=($(compgen -W "$comp_opt" -- "${COMP_WORDS[1]}"))
    fi
}

complete -F _g3kb_switch_completion g3kb-switch

