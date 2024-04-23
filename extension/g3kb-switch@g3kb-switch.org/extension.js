/// <reference types="@girs/gio-2.0/gio-2.0" />
/// <reference types="@girs/gnome-shell/dist/extensions" />
/// <reference types="@girs/gnome-shell/dist/ui/status/keyboard-ambient" />

'use strict';

import Gio from 'gi://Gio';
import { Extension } from 'resource:///org/gnome/shell/extensions/extension.js';
import * as Keyboard from 'resource:///org/gnome/shell/ui/status/keyboard.js';

const G3kbSwitchIface = `
<node>
    <interface name="org.g3kbswitch.G3kbSwitch">
        <method name="List">
            <arg type="b" direction="out" name="success"/>
            <arg type="s" direction="out" name="returnValue"/>
        </method>
        <method name="Get">
            <arg type="b" direction="out" name="success"/>
            <arg type="s" direction="out" name="returnValue"/>
        </method>
        <method name="Set">
            <arg type="u" direction="in" name="index"/>
            <arg type="b" direction="out" name="success"/>
            <arg type="s" direction="out" name="returnValue"/>
        </method>
    </interface>
</node>`;

export default class G3kbSwitchExtension extends Extension {
    constructor(metadata) {
        super(metadata);
        this._dbusImpl = Gio.DBusExportedObject.wrapJSObject(
            G3kbSwitchIface,
            this,
        );
    }

    enable() {
        this._dbusImpl.export(Gio.DBus.session, '/org/g3kbswitch/G3kbSwitch');
    }

    disable() {
        if (this._dbusImpl) this._dbusImpl.unexport();
    }

    List() {
        let returnValue;
        let success;
        let ids = [];

        try {
            /* BEWARE: Get() takes currentSource.index while here we simply put
             * counter i as the value of the key when iterating inputSources,
             * this should be correct as soon as currentSource.index drives
             * iteration order; to ensure correctness we could also put
             * inputSources[i].index instead of i */
            for (let i in Keyboard.getInputSourceManager().inputSources) {
                ids.push({
                    key: i,
                    value: Keyboard.getInputSourceManager().inputSources[i].id,
                });
            }
            returnValue = JSON.stringify(ids);
            returnValue = returnValue == undefined ? '' : returnValue;
            success = true;
        } catch (e) {
            returnValue = `${e}`;
            success = false;
        }

        return [success, returnValue];
    }

    Get() {
        let returnValue;
        let success;
        let idx;

        try {
            idx = Keyboard.getInputSourceManager().currentSource.index;
            returnValue = JSON.stringify(idx);
            success = true;
        } catch (e) {
            returnValue = `${e}`;
            success = false;
        }

        return [success, returnValue];
    }

    Set(index) {
        let returnValue;
        let success;

        try {
            Keyboard.getInputSourceManager().inputSources[index].activate();
            returnValue = '';
            success = true;
        } catch (e) {
            returnValue = `${e}`;
            success = false;
        }

        return [success, returnValue];
    }
}
