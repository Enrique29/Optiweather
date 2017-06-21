/*
 * guestbook view layer
 */

/**
 * contructor guestbook view
 * initializes layouts
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
						{ view: 'combo', label: 'SSID', value: 'Two', id: 'in_ssid', datatype: 'json', url: '/getssids'},
						{ view: 'text', label: 'Clave', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_password'},
						{ view: 'label', label: 'Dirección de reporte'},
						{ view: 'text', label: 'URL', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_urlreport'},
						{ view: 'label', label: 'Pin de acceso a monitor'},
						{ view: 'text', label: 'Pin', labelPosition: 'left', labelAlign: 'left', type: 'text', id: 'in_identitykey'},
						{ view: 'toggle',
							id: 'in_metodo', 
							align: 'center', 
							options: [
								{ value: '1', label: 'DHCP'},
								{ value: '2', label: 'Manual'}
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
						{ view: 'button', label: 'Enviar configuración', id: 'btn_send', type: 'round'}
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
	if($$('in_metodo').getValue()=="2"){
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
	/*
	var guestbook_view_this = this
	$$('btn_submit').attachEvent('onItemClick', function() {
		controller.newGuestbookEntry(guestbook_view_this.getNewModel())
	})
	*/
}

/**
 * generate a model from the form input data
 */
GuestbookView.prototype.getNewModel = function() {
	if ($$('form_new_entry').validate() === false) return false
	return {
		nickname: $$('txt_nickname').getValue(),
		createDate: new Date(),
		content: $$('txt_content').getValue()
	}
}

/**
 * add a new entry to the index dataview
 * @param mdl new model
 */
GuestbookView.prototype.addEntry = function(mdl) {
	//$$('dataview_index').add(mdl)
}

/**
 * @return the data of the list
 */
GuestbookView.prototype.getEntries = function() {
	return $$('dataview_index').data.serialize()
}

