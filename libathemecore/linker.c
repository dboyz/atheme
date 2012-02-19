/*
 * atheme-services: A collection of minimalist IRC services   
 * linker.c: Abstraction of the dynamic linking system.
 *
 * Copyright (c) 2005-2007 Atheme Project (http://www.atheme.org)           
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "atheme.h"
#include "linker.h"

#if defined(__HPUX__)
# define PLATFORM_SUFFIX ".sl"
#elif defined(_WIN32)
# define PLATFORM_SUFFIX ".dll"
#else
# define PLATFORM_SUFFIX ".so"
#endif

/*
 * linker_open_ext()
 *
 * Inputs:
 *       path to file to open
 *
 * Outputs:
 *       linker handle
 *
 * Side Effects:
 *       the extension is appended if it's not already there.
 *       a shared module is loaded into the application's memory space
 */
mowgli_module_t *linker_open_ext(const char *path, char *errbuf, int errlen)
{
	char *buf = smalloc(strlen(path) + 20);
	void *ret;

	mowgli_strlcpy(buf, path, strlen(path) + 20);

	if (!strstr(buf, PLATFORM_SUFFIX))
		mowgli_strlcat(buf, PLATFORM_SUFFIX, strlen(path) + 20);

	/* Don't try to open a file that doesn't exist. */
	struct stat s;
	if (0 != stat(buf, &s))
	{
		mowgli_strlcpy(errbuf, strerror(errno), errlen);
		return NULL;
	}

	ret = mowgli_module_open(buf);
	free(buf);

	if (!ret)
	{
		mowgli_strlcpy(errbuf, dlerror(), errlen);
	}

	return ret;
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
