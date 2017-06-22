/*
 * guestbook controller layer
 */

function GuestbookController() {
	this.guestbook_view = new GuestbookView()
	dhx.ui(this.guestbook_view.getRoot())
	
	this.guestbook_view.initEventHandler(this)

}

/**
 * submit a new guestbook entry
 */
GuestbookController.prototype.newGuestbookEntry = function(mdl) {
	console.log(mdl)
	if (!mdl) {
		dhx.notice("Tiene que completar los campos...")
	} else {
		this.guestbook_view.addEntry(mdl)
		//dhx.notice("New guestbook entry added.")
		//this.guestbook_view.showGuestbookIndexUI()
		//this.makePersistant()
	}
}

/**
 * saves the list's data in the local storage
 */


/**
 * reads the data from the local storage and fill the list
 */


