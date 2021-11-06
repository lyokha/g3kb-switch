'use strict';

const Gio = imports.gi.Gio;
const Keyboard = imports.ui.status.keyboard;

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

class Extension {
    constructor() {
        this._dbusImpl =
            Gio.DBusExportedObject.wrapJSObject(G3kbSwitchIface, this);
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
            for (let i in Keyboard.getInputSourceManager().inputSources) {
                ids.push({ key: i
                         , value: Keyboard.getInputSourceManager()
                                  .inputSources[i].id
                })
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

function init() {
    return new Extension();
}

