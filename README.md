# Authenticator

Authenticator is a [TOTP](http://en.wikipedia.org/wiki/Time-based_One-time_Password_Algorithm) based two-factor authentication manager for Pebble. It generates **T**ime-based **O**ne-**t**ime **P**assword for any service offering TOTP two-factor authentication including Google, Dropbox, Facebook, Microsoft, GitHub, Linode, etc.

This is a SDK 4 port of @Neal's pebble-authenticator, which is a fork of IEF's authenticator fork, which is a fork of pokey9000's twostep.

Option to vibrate when 5 seconds remain until the token renews and another one for new token.

Config is via Clay and thus local to phone and pebble. This does mean copies of the seed are stored on the phone.

Works on all pebbles (either using native pebble API to determine timezone/gmt-offset or JS on phone for older Aplite)

NOTE secret length is limited to 80-bits (10 bytes). This is below both the minimum and recommendation in [RFC 4226](https://tools.ietf.org/html/rfc6238)

> The length of the shared secret MUST be at least 128 bits.
> This document RECOMMENDs a shared secret length of 160 bits.

## Requirements

Either of:

* Cloudpebble (currently requires self hosting) and SDK3/4.
* local SDK, known to work with Pebble Tool v4.5 and SDK 4.3.

Running the `pebble` command assumes you have Pebble SDK installed configured to compile Pebble apps.
More info on how to set that up found [here](https://developer.rebble.io/developer.pebble.com/index.html).

## Configuration

Uses Clay.

See http://www.tomsguide.com/faq/id-2430350/find-authentication-code-pebble.html or https://web.archive.org/web/20170429103612/https://m.connectedly.com/how-get-your-two-step-verification-codes-your-pebble for how to get secrets onto phone/pebble. Extract/summary:

> So for example in Gmail, you'll need to login and go to https://accounts.google.com/b/0/SmsAuthSettings. Once there, select "Move to a different phone"
> 	
> Choose your phone type and click Continue
> 
> Now select the "Can't scan the barcode?" link to reveal the Secret -- in this case a string of 32 characters in the yellow box. Enter that code into the config for Pebble app.

## Install

	$ pebble build
	$ pebble install
