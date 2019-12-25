#!/usr/bin/env bash

_g3kb_switch_completion()
{
    if [ "${#COMP_WORDS[@]}" -gt 3 ] ; then
        return
    fi

    if [ "${#COMP_WORDS[@]}" -eq 3 ] ; then
        if [ "${COMP_WORDS[1]}" = "-s" ] ; then
            local IFS=$'\n'
            local -r cur=${COMP_WORDS[2]}
            local -r list=$(g3kb-switch -l)
            # it is very unlikely that names of layout groups may contain
            # spaces, nevertheless we prepare to process such names correctly
            COMPREPLY=( $(compgen -W "${list// /\\\\ }" -- "${cur// /\\\\ }") )
        fi
    else
        local IFS=$' \t\n'
        local -r comp_opt="-h -p -l -n -s"
        COMPREPLY=( $(compgen -W "$comp_opt" -- "${COMP_WORDS[1]}") )
    fi
}

complete -F _g3kb_switch_completion -o nosort g3kb-switch

