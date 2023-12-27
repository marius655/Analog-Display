const char index_html[] = R"rawliteral(
<html>
    <head>
        <meta charset='utf-8'>
        <title> Einrichtung </title>
        <style>
            section > *:not(h1){
                margin-left: 30px;
            }

            section:first-of-type > div:first-of-type{
                margin-left: 0;
            }

            h1{
                background-color: bisque;
                border-radius: 15px;
                width: fit-content;
                padding-left: 15px;
                padding-right: 15px;
            }

            section{
                margin-bottom: 50px;
            }

            .device_actions form{
                border: black solid 2px;
                width: fit-content;
                padding: 10px;
            }

            .device_actions > button{
                background-color:rgb(190, 239, 97);
                
                border-radius: 15px;
                padding: 3px;
                padding-left: 5px;
                padding-right: 5px;
                font-style: italic;
            }

            button:hover{
                cursor:pointer;
            }

            .device_actions > button:hover{
                border: black solid 3px;
            }

            .wlan_settings *{
                display: flex;
                flex-direction: column;
                width: fit-content;
                gap: 5px;
            }

            form *{
                width: fit-content;
            }

            .wlan_settings > select{
                margin-bottom: 10px;
            }

            .loader {
                border: 5px solid #f3f3f3; /* Light grey */
                border-top: 5px solid #3498db; /* Blue */
                border-radius: 50%;
                width: 10px;
                height: 10px;
                animation: spin 2s linear infinite;
                display: none;
                position: relative;
                left: -25;
            }

            .active_action{
                padding-right: 33px;
            }

            @keyframes spin {
                0% { transform: rotate(0deg); }
                100% { transform: rotate(360deg); }
            }

            #modbus > form{
                display: flex;
            }

            #modbus > form > label{
                margin-top: 10px;
            }

            #modbus > form > div{
                display: flex;
                flex-direction: column;
                margin-right: 10px;
            }

            #modbus > form{
                display: flex;
                flex-direction: column;
            }

            #modbus > form > div > label{
                margin-top: 5px;
            }
            
            #modbus > form > input{
                margin-bottom: 2px;
            }

            #mqtt > form{
                display: flex;
                flex-direction: column;
            }

            #mqtt > form > label:not(#mqtt>form>label:first-of-type){
                /* margin-bottom: 5px; */
                margin-top: 10px;
            }

        </style>
        <script>
            window.addEventListener("load", (event) => {
                let response_field = document.getElementById("response_field");
            });

            function httpPost( direction, callback ){
                fetch('http://192.168.10.65' + direction)
                    .then(response => {
                        // Check if the response status is in the 200-299 range (success)
                        if (response.ok) {
                            // Parse the response JSON if applicable
                            response_field.textContent = "";
                            return response.json();
                        } else {
                            // Handle the error or rejection of the request
                            //throw new Error('Failed to send POST request');                            
                            response_field.textContent = response.text;
                        }
                    })
                    .then( data => {
                        if( callback != null )
                            callback( data );
                    }) 
                    .catch(error => {
                        // Handle any errors that occurred during the request
                        console.error(error);
                    });
            }

            function httpPostFormElement( input_element ){
                // Use the fetch function to send the POST request
                let message = "";
                let parent = document.getElementById(input_element);
                let children = parent.children;
                for (let i = 0; i < children.length; i++) {
                    let tableChild = children[i];
                    if( tableChild instanceof HTMLInputElement && ( tableChild.getAttribute('type') == 'text' || tableChild.getAttribute('type') == 'number' )){
                        message += input_element + '_' + tableChild.id + '=' + tableChild.value + '&';
                    }
                    if( tableChild instanceof HTMLDivElement ){
                        for( let element of tableChild.children ){
                            if( element instanceof HTMLInputElement && ( element.getAttribute('type') == 'text' || element.getAttribute('type') == 'number') && tableChild.value !== '' ){
                                message += input_element + '_' + element.id + '=' + element.value + '&';
                            }
                        }
                    }
                }
                message = message.slice(0, -1);

                
                console.log(message);
                httpPost( "/settingsinput?" + message, null  )
            }

            function createWifiElements( networks ){
                let action = document.getElementById("action_loader");
                document.getElementById("action_header").classList.remove("active_action");
                action.style.display = "none";


                //console.log( networks );
                let listSelection = document.getElementById("wifiSelect");
                
                let hasItems = false;
                for( const network of networks ){
                    if (network.hasOwnProperty("wifiName") &&
                        network.hasOwnProperty("signalStrength") ){
                        if(!hasItems)
                            hasItems = true;

                        let child = document.createElement("option");
                        child.id = "wifiSelect_option";
                        child.textContent = network.wifiName + "(" + network.signalStrength + "dbm)";
                        listSelection.appendChild(child)
                    } 
                }

                if( networks.length >= 2 ){
                    listSelection.setAttribute("size", "2");
                }

                if(hasItems){
                    let standardItem = document.getElementById("wifiSelect_standard");
                    standardItem.style.display = "none";
                }
            }

            function getNetworks(){
                httpPost("/networks", createWifiElements );

                let action = document.getElementById("action_loader");
                document.getElementById("action_header").classList.add("active_action");
                action.style.display = "block";
            }

            function handleWifiSelection( element ){
                let text = element.options[element.selectedIndex].text;
                text = text.substring(0, text.indexOf('('));
                console.log(text);
                let form = document.getElementById('form_wlan_settings');
                let child = form.querySelector('#SSID');
                child.value = text;
            }

            function handleModbusJobSelection( element ){
                let value = element.options[element.selectedIndex].value;
                if( value === "client" ){
                    let clientSettings = document.getElementById("form_modbus_settings_client_job");
                    clientSettings.style.display = "flex";
                    console.log("should be client");
                }
                if( value === "server" ){
                    let clientSettings = document.getElementById("form_modbus_settings_client_job");
                    clientSettings.style.display = "none";
                    console.log( "should be server" );
                }
            }
        </script>
    </head>
    <body>
        <section>
            <div style="display: flex; align-items: center;">
                <h1 id="action_header">Actions</h1>
                <div id="action_loader" class="loader"></div> 
            </div>
            <div class='device_actions'>
                <button onclick='httpPost("/reboot")'>Reboot device</button>
                <button onclick='getNetworks()'>Refresh Networks</button>

                <p id="action_networkRefresh" style="display: none;">Please wait: Network Refresh</p>
            </div>
            <p id="response_field"></p>
        </section>
        <section>
            <h1>Update</h1>
            <form method='POST' action='/update' enctype='multipart/form-data'>
                Firmware:<br>
                <input type='file' accept='.bin,.bin.gz' name='firmware'>
                <input type='submit' value='Update Firmware'>
            </form>
            <form method='POST' action='/update' enctype='multipart/form-data'>
                FileSystem:<br>
                <input type='file' accept='.bin,.bin.gz' name='filesystem'>
                <input type='submit' value='Update FileSystem'>
            </form>
        </section>
        <section>
            <h1> WLAN-Settings </h1>
            <div class='wlan_settings'>
                <select onchange="handleWifiSelection(this)" id="wifiSelect">
                    <option onse id="wifiSelect_standard">pls refresh networks to see available networks</option>
                </select>
                <form id='form_wlan_settings' action='/settingsinput'>
                    <label>WLAN SSID:</label>
                    <input type='text' id='SSID'>
                    <label>WLAN Password:</label>
                    <input type='text' id='WLAN_KEY'>
                </form>
                <button onclick='httpPostFormElement("form_wlan_settings")'>Speichern</button>
            </div>
        </section>
        <section id="mqtt">
            <h1>MQTT</h1>
            <form id="form_mqtt_settings">
                <label>IP MQTT Broker</label>
                <input type="text" id="mqtt_broker_address">
                <label>Port</label>
                <input type="number" id="mqtt_broker_port">
                <label>Password</label>
                <input type="text" id="mqtt_broker_password">
            </form>
            <button onclick='httpPostFormElement("form_mqtt_settings")' >Speichern</button>
        </section>
        <section id="modbus">
            <h1>Modbus</h1>

            <select name="job" id="modbus_jobselect" form="form_modbus_settings" onchange="handleModbusJobSelection(this)">
                <option value="client">Client</option>
                <option value="server">Server</option>
            </select>
            <form id="form_modbus_settings">
                <div id="form_modbus_settings_client_job">
                    <label>Server IP</label>
                    <input id="modbus_server_ip" type="text">
                    <label>Server Port</label>
                    <input id="modbus_server_port" type="number">
                </div>

                <label>Register</label>
                <input type="text" id="modbus_adress1">
                <input type="text" id="modbus_adress2">
                <input type="text" id="modbus_adress3">
                <input type="text" id="modbus_adress4">
                <input type="text" id="modbus_adress5">
            </form>
            <button onclick='httpPostFormElement("form_modbus_settings")' >Speichern</button>
        </section>
    </body>
</html>
)rawliteral";
