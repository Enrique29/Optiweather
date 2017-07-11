/*
 * guestbook view layer
 */

/**
 * contructor guestbook view
 * initializes layocs
 */
function GuestbookView() {
	
	this.ui_guestbook_entry = {
		view: 'layout',
		type: 'line',
		animate:{type:"slide", subtype:"vertical"},
		rows: [
		{ view: 'layout', type: 'wide',
			rows: [
				{ view: 'toolbar', type: 'MainBar',
					elements: [
						{ view: 'label', label: 'OPTIWEATHER', id: 'control_label_0'}
					]
				},
				{	
					view: 'form', 
					id: 'form_main',
					scroll: true,
					elements: [
						{ view: 'combo', label: 'SSID', value: 'Two', id: 'in_ssid', datatype: 'json', url: 'getssids'},
						{ view: 'text', label: 'Clave', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_password'},
						{ view: 'label', label: 'Dirección de reporte'},
						{ view: 'text', label: 'URL', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_urlreport'},
						{ view: 'label', label: 'Pin de acceso a monitor'},
						{ view: 'text', label: 'Pin', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_identitykey'},
						{ view: 'toggle',
							id: 'in_metodo', 
							align: 'center', 
							options: [
								{ value: '0', label: 'DHCP'},
								{ value: '1', label: 'Manual'}
							] 
						},
						{ view: 'text', label: 'IP', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_ip'},
						{ view: 'text', label: 'Gateway', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_gateway'},
						{ view: 'text', label: 'Mask', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_mask'},
						{ view: 'text', label: 'DNS1', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_dns1'},
						{ view: 'text', label: 'DNS2', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_dns2'}
					]
				},
				{ view: 'toolbar', type: 'MainBar',
					elements: [
						{ view: 'button', label: 'Enviar configuración', id: 'btn_submit', type: 'round'}
					]
				}
			]	
		}
	],
		id: 'ui_guestbook_entry'
	}
	
	this.ui_root = {
		id: 'app', view: 'multiview',
		rows: [
			this.ui_guestbook_entry
		]
	}
}

/**
 * @return the root layout
 */
GuestbookView.prototype.getRoot = function() {
	return this.ui_root
}

/**
 * shows the guetsbook entry ui
 */
 
GuestbookView.prototype.showGuestbookEntryUI = function() {
	$$('in_ip').hide();
	$$('in_gateway').hide();
	$$('in_mask').hide();
	$$('in_dns1').hide();
	$$('in_dns2').hide();
	if($$('in_metodo').getValue()=="1"){
		$$('in_ip').show();
		$$('in_gateway').show();
		$$('in_mask').show();
		$$('in_dns1').show();
		$$('in_dns2').show();
		dhx.notice("Ha activado el método manual");
	}
	
}

/**
 * shows the guetsbook entry ui
 */
GuestbookView.prototype.showGuestbookIndexUI = function() {
	$$('ui_guestbook_entry').show()
}

GuestbookView.prototype.initEventHandler = function(controller) {
	
	// sets event handler 'new guestbook entry'
	$$('in_metodo').attachEvent('onItemClick',this.showGuestbookEntryUI);
	
	this.showGuestbookEntryUI();
	
	// 'back'
	//$$('btn_back').attachEvent('onItemClick', this.showGuestbookIndexUI)
	
	// 'submit'
	
	var guestbook_view_this = this
	
	$$('btn_submit').attachEvent('onItemClick', function() {
		controller.newGuestbookEntry(guestbook_view_this.getNewModel())
	})
	
}

/**
 * generate a model from the form input data
 */
function isnulldefaul(p){
	if (p==null) return ""
	return p;
	
} 
 
GuestbookView.prototype.getNewModel = function() {
	var rtn = "&in_ssid="+isnulldefaul($$('in_ssid').getValue())+
		"&in_password="+isnulldefaul($$('in_password').getValue())+
		"&in_urlreport="+isnulldefaul($$('in_urlreport').getValue())+
		"&in_identitykey="+isnulldefaul($$('in_identitykey').getValue())+
		"&in_metodo="+isnulldefaul($$('in_metodo').getValue())+
		"&in_ip="+isnulldefaul($$('in_ip').getValue())+
		"&in_gateway="+isnulldefaul($$('in_gateway').getValue())+
		"&in_mask="+isnulldefaul($$('in_mask').getValue())+
		"&in_dns1="+isnulldefaul($$('in_dns1').getValue())+
		"&in_dns2="+isnulldefaul($$('in_dns2').getValue())			
	return rtn;
		
}

/**
 * add a new entry to the index dataview
 * @param mdl new model
 */
GuestbookView.prototype.addEntry = function(mdl) {
	//$$('dataview_index').add(mdl)
	
	var n = dhx.ajax().sync().get("/salvarDatos?n=1"+mdl);
	if (n.status!=200){
		if (n.status==404)dhx.alert ("Pagina no encontrada..");
		if (n.status==500)dhx.alert ("Error en la pagina..");
		return;
	}
	
	n = JSON.parse(n.responseText);
	
	if (n.status!="ok"){
		dhx.alert (n.statusText);
		return;
	}



	var n = dhx.ajax().sync().get("/validar");
	if (n.status!=200){
		if (n.status==404)dhx.alert ("Pagina no encontrada..");
		if (n.status==500)dhx.alert ("Error en la pagina..");
		return;
	}
	
	n = JSON.parse(n.responseText);
	
	if (n.status!="ok"){
		dhx.alert (n.statusText);
		return;
	}
	dhx.alert ("La configuración ha sido exitosa, ya puede desconectarse");

	console.log (n);

}

/**
 * @return the data of the list
 */
GuestbookView.prototype.getEntries = function() {
	return $$('dataview_index').data.serialize()
}

