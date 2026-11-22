guard();
header();

async function loadContacts() {
    try {
        const contacts = await api(`/api/contacts?user=${encodeURIComponent(user)}`);
        const list = $('contactList');

        if (!contacts.length) {
            list.innerHTML = '<p class="muted">No contacts yet. Add someone you trust above.</p>';
            return;
        }
//contact

        list.innerHTML = contacts.map((contact) => `
            <div class="contact-row">
                <div class="contact-avatar">${escapeHTML(contact.name.charAt(0).toUpperCase())}</div>
                <div class="contact-details">
                    <b>${escapeHTML(contact.name)}</b>
                    <span>${escapeHTML(contact.phone)}</span>
                </div>
                <button class="button-light remove-button" onclick="removeContact(${JSON.stringify(contact.name)})">Remove</button>
            </div>
        `).join('');
    } catch (error) {
        $('contactList').innerHTML = '<p class="message">Could not load contacts. Please try again.</p>';
    }
}

async function addContact(event) {
    event.preventDefault();

    const name = $('contactName').value.trim();
    const phone = $('contactPhone').value.trim();

    if (!name || !phone) {
    $('contactMessage').textContent = 'Please enter name and phone number.';
    return;
}
//length checker
if (name.length < 2) {
    $('contactMessage').textContent = 'Name must contain at least 2 characters.';
    return;
}

if (!/^[0-9+\-\s]{10,15}$/.test(phone)) {
    $('contactMessage').textContent = 'Please enter a valid phone number.';
    return;
}

    const result = await api('/api/contacts/add', { user, name, phone });
    $('contactMessage').textContent = result.msg || (result.ok ? 'Contact added.' : 'Could not add contact.');

    if (result.ok) {
        $('contactForm').reset();
        loadContacts();
    }
}

async function removeContact(name) {
    const result = await api('/api/contacts/remove', { user, name });
    $('contactMessage').textContent = result.ok ? 'Contact removed.' : 'Could not remove contact.';
    loadContacts();
}

$('contactForm').addEventListener('submit', addContact);
loadContacts();
