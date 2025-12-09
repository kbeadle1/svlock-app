/* svlock.swg */
%module svlock

%include "typemaps.i"
%include "stdint.i"

%{
#include "../svlock/svlock.h"
%}

typedef unsigned long long off_t;

/* Need this for the pointer to arrary of strings returned from svlock_topo_get_*_names() */

%typemap(in) char *** (char **temp) {
/* char *** in */

if (PyList_Check($input)) {
	int size = PyList_Size($input);
	/* Empty the list */
	PyList_SetSlice($input,0,size,NULL);
	size = PyList_Size($input);
} else {
	PyErr_SetString(PyExc_TypeError,"not a list");
	return NULL;
}

arg1 = &temp;
}

%typemap(argout,fragment="SWIG_FromCharPtr") char ***{
int i;
PyObject *resstr;

/* char *** out */

for(i=0;i<result;i++) {
	resstr = SWIG_FromCharPtr(temp$argnum[i]);
	PyList_Append($input,resstr);
	}
}

/* This is needed for svlock_topo_get_thread_name_by_apic_id() */
%typemap(argout) (int apic_id, char *result, size_t res_size) {
/* svlock_topo_get_thread_name_by_apic_id out */
Py_XDECREF($result);

$result = PyBytes_FromStringAndSize($2,strlen($2));
}

/* This is needed for svlock_topo_get_mmconfig_by_domain() */
%apply uint64_t *OUTPUT { uint64_t *base, uint64_t *len }

/* This is needed for svlock_cpuid() */
%apply uint32_t *OUTPUT { uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx }

/* These are needed for svlock_read_phys_mem_*_bytes() and svlock_reg_*() */
%apply uint8_t *OUTPUT { uint8_t *value }
%apply uint16_t *OUTPUT { uint16_t *value }
%apply uint32_t *OUTPUT { uint32_t *value }
%apply uint64_t *OUTPUT { uint64_t *value }

/* These are needed for svlock_topo_get_pci_data() */
%typemap(argout) (const char *pcidev_name, struct pci_data *data) {
/* svlock_topo_get_pci_data out */
}
%apply struct pci_data *OUTPUT { const char *pcidev_name, struct pci_data *data }

/* These are needed for svlock_topo_get_*_data() */
%typemap(argout) (const char *target_name, struct target_data *data) {
/* svlock_topo_get_target_data out */
}
%apply struct target_data *OUTPUT { const char *target_name, struct target_data *data }

%include "svlock.i"
