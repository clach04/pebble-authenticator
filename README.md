# Authenticator

Authenticator is a [TOTP](http://en.wikipedia.org/wiki/Time-based_One-time_Password_Algorithm) based two-factor authentication manager for Pebble. It generates **T**ime-based **O**ne-**t**ime **P**assword for any service offering TOTP two-factor authentication including Google, Dropbox, Facebook, Microsoft, GitHub, Linode, etc.

This is a SDK 3.0 port of @Neal's pebble-authenticator, which is a fork of IEF's authenticator fork, which is a fork of pokey9000's twostep.

Option to vibrate when 5 seconds remain until the token renews and another one for new token.

Config is via Clay and thus local to phone and pebble. This does mean copies of the seed are stored on the phone.

Works on all pebbles (either using native pebble API to determine timezone/gmt-offset or JS on phone for older Aplite)

## Requirements

Cloudpebble account and SDK3. NOTE untested with local SDK.

Running the `pebble` command assumes you have Pebble SDK 3.0 installed configured to compile Pebble apps.
More info on how to set that up found [here](https://developer.getpebble.com/2/getting-started/).

## Configuration

Uses Clay.

See https://m.connectedly.com/how-get-your-two-step-verification-codes-your-pebble for how to get secrets onto phone/pebble.

## Install

	$ pebble build
	$ pebble install
