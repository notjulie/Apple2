
function jsr(address) {
	return [
		0x20,
		address & 0xFF,
		address >> 8
	];
}

