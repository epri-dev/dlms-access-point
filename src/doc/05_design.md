# How it works {#design}
The software is composed of a number of pieces, which together make up the simulation.  The components are:

  1. Meter simulator(s)
  2. Head-End System (HES) simulator
  3. Access Point (AP) simulator

Each of the components is described in more detail below.

## Meter simulator
The meter simulator uses the EPRI DLMS/COSEM library and contains a small collection of standard COSEM objects.  The objects include a EPRI::LinuxClock component and a EPRI::LinuxDisconnect object.  The simulator is intended to simulate only network traffic rather than a real device, so the objects exist but the functionality and the data in them is not, and is not intended to be realistic.  The meter simulator listens on the standard DLMS/COSEM port of 4059.

It implements the following classes of objects:

### Data class_id = 1, version = 0 { 0, 0, 96, 1, {0, 9}, 255 }
    Get and Set both implemented

### Clock class_id = 8, version = 0 { 0, 0, 1, 0, 0, 255 }
    all present but unimplemeneted

<table>
<caption id="Clock_attributes">Clock Attributes</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>2<td>ATTR_TIME<td>read implemented
<tr><td>3<td>ATTR_TIME_ZONE<td>
<tr><td>4<td>ATTR_STATUS<td>
<tr><td>5<td>ATTR_DST_BEGIN<td>
<tr><td>6<td>ATTR_DST_END<td>
<tr><td>7<td>ATTR_DST_DEVIATION<td>
<tr><td>8<td>ATTR_DST_ENABLED<td>
<tr><td>9<td>ATTR_CLOCK_BASE<td>
</table>

<table>
<caption id="Clock_Methods">Clock Methods</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>1<td>METHOD_ADJUST_TO_QUARTER<td>
<tr><td>2<td>METHOD_ADJUST_TO_QUARTER<td>
<tr><td>3<td>METHOD_ADJUST_TO_MEAS_PERIOD<td>
<tr><td>4<td>METHOD_ADJUST_TO_MINUTE<td>
<tr><td>5<td>METHOD_ADJUST_TO_PRESET_TIME<td>
<tr><td>6<td>METHOD_PRESET_ADJUSTING_TIME<td>
<tr><td>7<td>METHOD_SHIFT_TIME<td>
</table>

### Association SN class_id = 12, version = 4 {0, 0, 40, 0, {0, 1}, 255}
nothing implemented

### Association LN class_id = 15, version = 4 {0, 0, 40, 0, {0, 1}, 255}
<table>
<caption id="Association_LN_Attributes">Association LN Attributes</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>2<td>ATTR_OBJ_LIST<td> not implemented; returns unavailable
<tr><td>3<td>ATTR_PARTNERS_ID<td> returns clientSAP, ServerSAP
<tr><td>4<td>ATTR_CON_NAME<td> return application context name
<tr><td>5<td>ATTR_XDLMS_CON_INFO<td> implemented
<tr><td>6<td>ATTR_AUTH_MECH_NAME<td> implemented
<tr><td>7<td>ATTR_SECRET<td> read_write_denied
<tr><td>8<td>ATTR_STATUS<td> implemented
<tr><td>9<td>ATTR_SECURITY_SETUP_REF<td> read_write_denied
<tr><td>10<td>ATTR_USER_LIST<td> not implemented
<tr><td>11<td>ATTR_CURRENT_USER<td> not implemented
</table>

### Disconnect class_id = 70, version = 0 { 0, 0, 96, 3, 10, 255 }
<table>
<caption id="Disconnect_attributes">Disconnect Attributes</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>2<td>ATTR_OUTPUT_STATE<td> implemented
<tr><td>3<td>ATTR_CONTROL_STATE<td> implemented
<tr><td>4<td>ATTR_CONTROL_MODE<td> implemented
</table>

<table>
<caption id="Disconnect_Methods">Disconnect Methods</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>1<td>METHOD_REMOTE_DISCONNECT<td> implemented
<tr><td>2<td>METHOD_REMOTE_RECONNECT<td> implemented
</table>

### ImageTransfer class_id = 18, version = 0 { 0, 0, 44, 0, 0, 255 }
<table>
<caption id="ImageTransfer_attributes">ImageTransfer Attributes</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>2<td>ATTR_IMAGE_BLOCK_SIZE<td> implemented
<tr><td>3<td>ATTR_IMAGE_TRANSFERRED_BLOCKS_STATUS<td> implemented
<tr><td>4<td>ATTR_IMAGE_FIRST_NOT_TRANSFERRED_BLOCK_NUMBER<td> implemented
<tr><td>5<td>ATTR_IMAGE_TRANSFER_ENABLED<td> implemented
<tr><td>6<td>ATTR_IMAGE_TRANSFER_STATUS<td> implemented
<tr><td>7<td>ATTR_IMAGE_TO_ACTIVATE_INFO<td> implemented
</table>

<table>
<caption id="ImageTransfer_Methods">ImageTransfer Methods</caption>
<tr><th>Number<th>Name<th>Status
<tr><td>1<td>METHOD_IMAGE_TRANSFER_INITIATE<td> implemented
<tr><td>2<td>METHOD_IMAGE_BLOCK_TRANSFER<td> implemented
<tr><td>3<td>METHOD_IMAGE_VERIFY<td> implemented
<tr><td>4<td>METHOD_IMAGE_ACTIVATE<td> implemented
</table>

## HES simulator
The Head-End System simulator here has only one job, which is to communicate with the simulated meters.  At the moment, the HES only has three things that it can do:

  1. get the time of day from the Clock object (implemented via the EPRI::LinuxClock class)
  2. operate the connect/disconnect via the Disconnect object (implemented via the EPRI::LinuxDisconnect class)
  3. write a large amount of data (implemented via the EPRI::LinuxImageTransfer class)
  4. read a large amount of data (via the EPRI::LinuxData class)

## Access Point (AP) simulator
The AP can operate in any of three Modes:

 1. as a lower layer router only (Mode 1)
 2. as a store-and-forward network (Mode 2)
 3. as a hybrid device performing both functions (Mode 3)


See [Introduction](@ref mainpage) for more information on these modes.
