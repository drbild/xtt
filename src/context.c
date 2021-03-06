/******************************************************************************
 *
 * Copyright 2018 Xaptum, Inc.
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License
 *
 *****************************************************************************/

#include <xtt/context.h>
#include <xtt/crypto_wrapper.h>
#include <xtt/daa_wrapper.h>
#include <xtt/error_codes.h>

#include "internal/crypto_utils.h"
#include "internal/message_utils.h"
#include "internal/byte_utils.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

xtt_error_code
xtt_initialize_server_handshake_context(struct xtt_server_handshake_context* ctx_out,
                                        xtt_version version,
                                        xtt_suite_spec suite_spec)
{
    if (ctx_out == NULL)
        return XTT_ERROR_NULL_BUFFER;

    if (XTT_VERSION_ONE != version)
        return XTT_ERROR_UNKNOWN_VERSION;

    ctx_out->base.version = version;

    ctx_out->base.suite_spec = suite_spec;

    switch (suite_spec) {
        case XTT_X25519_LRSW_ED25519_CHACHA20POLY1305_SHA512:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_sha512;

            ctx_out->base.encrypt = encrypt_chacha;
            ctx_out->base.decrypt = decrypt_chacha;

            ctx_out->base.hash = xtt_crypto_hash_sha512;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_sha512);
            ctx_out->base.mac_length = sizeof(xtt_chacha_mac);
            ctx_out->base.key_length = sizeof(xtt_chacha_key);
            ctx_out->base.iv_length = sizeof(xtt_chacha_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->read_longterm_key = read_longterm_key_ed25519;

            ctx_out->verify_client_longterm_signature = verify_server_signature_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        case XTT_X25519_LRSW_ED25519_CHACHA20POLY1305_BLAKE2B:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_blake2b;

            ctx_out->base.encrypt = encrypt_chacha;
            ctx_out->base.decrypt = decrypt_chacha;

            ctx_out->base.hash = xtt_crypto_hash_blake2b;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_blake2b);
            ctx_out->base.mac_length = sizeof(xtt_chacha_mac);
            ctx_out->base.key_length = sizeof(xtt_chacha_key);
            ctx_out->base.iv_length = sizeof(xtt_chacha_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->read_longterm_key = read_longterm_key_ed25519;

            ctx_out->verify_client_longterm_signature = verify_server_signature_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        case XTT_X25519_LRSW_ED25519_AES256GCM_SHA512:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_sha512;

            ctx_out->base.encrypt = encrypt_aes256;
            ctx_out->base.decrypt = decrypt_aes256;

            ctx_out->base.hash = xtt_crypto_hash_sha512;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_sha512);
            ctx_out->base.mac_length = sizeof(xtt_aes256_mac);
            ctx_out->base.key_length = sizeof(xtt_aes256_key);
            ctx_out->base.iv_length = sizeof(xtt_aes256_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->read_longterm_key = read_longterm_key_ed25519;

            ctx_out->verify_client_longterm_signature = verify_server_signature_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        case XTT_X25519_LRSW_ED25519_AES256GCM_BLAKE2B:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_blake2b;

            ctx_out->base.encrypt = encrypt_aes256;
            ctx_out->base.decrypt = decrypt_aes256;

            ctx_out->base.hash = xtt_crypto_hash_blake2b;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_blake2b);
            ctx_out->base.mac_length = sizeof(xtt_aes256_mac);
            ctx_out->base.key_length = sizeof(xtt_aes256_key);
            ctx_out->base.iv_length = sizeof(xtt_aes256_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->read_longterm_key = read_longterm_key_ed25519;

            ctx_out->verify_client_longterm_signature = verify_server_signature_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        default:
            return XTT_ERROR_UNKNOWN_CRYPTO_SPEC;
    }
}

xtt_error_code
xtt_initialize_client_handshake_context(struct xtt_client_handshake_context* ctx_out,
                                        xtt_version version,
                                        xtt_suite_spec suite_spec)
{
    if (ctx_out == NULL)
        return XTT_ERROR_NULL_BUFFER;

    if (XTT_VERSION_ONE != version)
        return XTT_ERROR_UNKNOWN_VERSION;

    ctx_out->base.version = version;

    ctx_out->base.suite_spec = suite_spec;

    switch (suite_spec) {
        case XTT_X25519_LRSW_ED25519_CHACHA20POLY1305_SHA512:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_sha512;


            ctx_out->base.encrypt = encrypt_chacha;
            ctx_out->base.decrypt = decrypt_chacha;

            ctx_out->base.hash = xtt_crypto_hash_sha512;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_sha512);
            ctx_out->base.mac_length = sizeof(xtt_chacha_mac);
            ctx_out->base.key_length = sizeof(xtt_chacha_key);
            ctx_out->base.iv_length = sizeof(xtt_chacha_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->verify_server_signature = verify_server_signature_ed25519;

            ctx_out->copy_longterm_key = copy_longterm_key_ed25519;
            
            ctx_out->compare_longterm_keys = compare_longterm_keys_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            ctx_out->longterm_sign = longterm_sign_ed25519;

            if (0 != xtt_crypto_create_ed25519_key_pair(&ctx_out->longterm_key.ed25519, &ctx_out->longterm_private_key.ed25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        case XTT_X25519_LRSW_ED25519_CHACHA20POLY1305_BLAKE2B:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_blake2b;


            ctx_out->base.encrypt = encrypt_chacha;
            ctx_out->base.decrypt = decrypt_chacha;

            ctx_out->base.hash = xtt_crypto_hash_blake2b;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_blake2b);
            ctx_out->base.mac_length = sizeof(xtt_chacha_mac);
            ctx_out->base.key_length = sizeof(xtt_chacha_key);
            ctx_out->base.iv_length = sizeof(xtt_chacha_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->verify_server_signature = verify_server_signature_ed25519;

            ctx_out->copy_longterm_key = copy_longterm_key_ed25519;
            
            ctx_out->compare_longterm_keys = compare_longterm_keys_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            ctx_out->longterm_sign = longterm_sign_ed25519;

            if (0 != xtt_crypto_create_ed25519_key_pair(&ctx_out->longterm_key.ed25519, &ctx_out->longterm_private_key.ed25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        case XTT_X25519_LRSW_ED25519_AES256GCM_SHA512:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_sha512;


            ctx_out->base.encrypt = encrypt_aes256;
            ctx_out->base.decrypt = decrypt_aes256;

            ctx_out->base.hash = xtt_crypto_hash_sha512;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_sha512);
            ctx_out->base.mac_length = sizeof(xtt_aes256_mac);
            ctx_out->base.key_length = sizeof(xtt_aes256_key);
            ctx_out->base.iv_length = sizeof(xtt_aes256_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->verify_server_signature = verify_server_signature_ed25519;

            ctx_out->copy_longterm_key = copy_longterm_key_ed25519;
            
            ctx_out->compare_longterm_keys = compare_longterm_keys_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            ctx_out->longterm_sign = longterm_sign_ed25519;

            if (0 != xtt_crypto_create_ed25519_key_pair(&ctx_out->longterm_key.ed25519, &ctx_out->longterm_private_key.ed25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        case XTT_X25519_LRSW_ED25519_AES256GCM_BLAKE2B:
            ctx_out->base.hash_out_buffer = (unsigned char*)&ctx_out->base.hash_out_buffer_raw;
            ctx_out->base.inner_hash = (unsigned char*)&ctx_out->base.inner_hash_raw;

            ctx_out->base.shared_secret_buffer = (unsigned char*)&ctx_out->base.shared_secret_raw;
            ctx_out->base.handshake_secret = (unsigned char*)&ctx_out->base.handshake_secret_raw;
            ctx_out->base.prf_key = (unsigned char*)&ctx_out->base.prf_key_raw;
            memset(ctx_out->base.prf_key, 0, sizeof(ctx_out->base.prf_key_raw));

            ctx_out->base.copy_dh_pubkey = copy_dh_pubkey_x25519;

            ctx_out->base.do_diffie_hellman = do_diffie_hellman_x25519;
            ctx_out->base.prf = xtt_crypto_prf_blake2b;


            ctx_out->base.encrypt = encrypt_aes256;
            ctx_out->base.decrypt = decrypt_aes256;

            ctx_out->base.hash = xtt_crypto_hash_blake2b;

            ctx_out->base.longterm_key_length = sizeof(xtt_ed25519_pub_key);
            ctx_out->base.longterm_key_signature_length = sizeof(xtt_ed25519_signature);
            ctx_out->base.shared_secret_length = sizeof(xtt_x25519_shared_secret);
            ctx_out->base.hash_length = sizeof(xtt_blake2b);
            ctx_out->base.mac_length = sizeof(xtt_aes256_mac);
            ctx_out->base.key_length = sizeof(xtt_aes256_key);
            ctx_out->base.iv_length = sizeof(xtt_aes256_nonce);

            ctx_out->base.tx_sequence_num = 0;
            ctx_out->base.rx_sequence_num = 0;

            ctx_out->verify_server_signature = verify_server_signature_ed25519;

            ctx_out->copy_longterm_key = copy_longterm_key_ed25519;
            
            ctx_out->compare_longterm_keys = compare_longterm_keys_ed25519;

            if (0 != xtt_crypto_create_x25519_key_pair(&ctx_out->base.dh_pub_key.x25519, &ctx_out->base.dh_priv_key.x25519))
                return XTT_ERROR_CRYPTO;

            ctx_out->longterm_sign = longterm_sign_ed25519;

            if (0 != xtt_crypto_create_ed25519_key_pair(&ctx_out->longterm_key.ed25519, &ctx_out->longterm_private_key.ed25519))
                return XTT_ERROR_CRYPTO;

            return XTT_ERROR_SUCCESS;
        default:
            return XTT_ERROR_UNKNOWN_CRYPTO_SPEC;
    }
}

xtt_error_code
xtt_initialize_server_cookie_context(struct xtt_server_cookie_context* ctx)
{
    // We're not currently using anything in the cookie context, so NOOP.
    (void)ctx;
    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_initialize_server_certificate_context_ed25519(struct xtt_server_certificate_context *ctx_out,
                                                  const unsigned char *serialized_certificate,
                                                  xtt_ed25519_priv_key *private_key)
{
    ctx_out->sign = sign_server_ed25519;

    ctx_out->signature_length = sizeof(xtt_ed25519_signature);

    ctx_out->private_key.ed25519 = *private_key;

    ctx_out->serialized_certificate = (struct xtt_server_certificate_raw_type*)ctx_out->serialized_certificate_raw;
    memcpy(ctx_out->serialized_certificate_raw,
           serialized_certificate,
           xtt_server_certificate_length_fromsignaturetype(XTT_SERVER_SIGNATURE_TYPE_ED25519));

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_initialize_server_root_certificate_context_ed25519(struct xtt_server_root_certificate_context *cert_out,
                                                       xtt_certificate_root_id *id,
                                                       xtt_ed25519_pub_key *public_key)
{
    cert_out->verify_signature = verify_root_ed25519;

    cert_out->type = XTT_SERVER_SIGNATURE_TYPE_ED25519;

    cert_out->id = *id;

    cert_out->public_key.ed25519 = *public_key;

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_initialize_daa_group_public_key_context_lrsw(struct xtt_daa_group_public_key_context *ctx_out,
                                                 const unsigned char *basename,
                                                 uint16_t basename_length,
                                                 xtt_daa_group_pub_key_lrsw *gpk)
{
    ctx_out->verify_signature = verify_lrswTPM;

    ctx_out->gpk.lrsw = *gpk;

    if (basename_length > sizeof(ctx_out->basename))
        return XTT_ERROR_BAD_INIT;
    memcpy(ctx_out->basename,
           basename,
           basename_length);
    ctx_out->basename_length = basename_length;

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_initialize_daa_context_lrswTPM(struct xtt_daa_context *ctx_out,
                                   xtt_daa_group_id *gid,
                                   xtt_daa_credential_lrsw *cred,
                                   const unsigned char *basename,
                                   uint16_t basename_length,
                                   struct xtt_daa_tpm_context *tpm_context)
{
    ctx_out->sign = sign_lrswTPM;

    ctx_out->gid = *gid;

    ctx_out->cred.lrsw = *cred;

    if (basename_length > sizeof(ctx_out->basename))
        return XTT_ERROR_BAD_INIT;
    memcpy(ctx_out->basename,
           basename,
           basename_length);
    ctx_out->basename_length = basename_length;

    ctx_out->tpm_context = tpm_context;

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_initialize_daa_context_lrsw(struct xtt_daa_context *ctx_out,
                                xtt_daa_group_id *gid,
                                xtt_daa_priv_key_lrsw *priv_key,
                                xtt_daa_credential_lrsw *cred,
                                const unsigned char *basename,
                                uint16_t basename_length)
{
    ctx_out->sign = sign_lrsw;

    ctx_out->gid = *gid;

    ctx_out->priv_key.lrsw = *priv_key;

    ctx_out->cred.lrsw = *cred;

    if (basename_length > sizeof(ctx_out->basename))
        return XTT_ERROR_BAD_INIT;
    memcpy(ctx_out->basename,
           basename,
           basename_length);
    ctx_out->basename_length = basename_length;

    ctx_out->tpm_context = NULL;

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_get_clients_longterm_key_ed25519(xtt_ed25519_pub_key *longterm_key_out,
                                     const struct xtt_server_handshake_context *handshake_context)
{
    memcpy(longterm_key_out,
           handshake_context->clients_longterm_key.ed25519.data,
           sizeof(xtt_ed25519_pub_key));

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_get_my_longterm_key_ed25519(xtt_ed25519_pub_key *longterm_key_out,
                                const struct xtt_client_handshake_context *handshake_context)
{
    memcpy(longterm_key_out,
           handshake_context->longterm_key.ed25519.data,
           sizeof(xtt_ed25519_pub_key));

    return XTT_ERROR_SUCCESS;
}

xtt_error_code
xtt_get_my_longterm_private_key_ed25519(xtt_ed25519_priv_key *longterm_key_priv_out,
                                        const struct xtt_client_handshake_context *handshake_context)
{
    memcpy(longterm_key_priv_out,
           handshake_context->longterm_private_key.ed25519.data,
           sizeof(xtt_ed25519_priv_key));

    return XTT_ERROR_SUCCESS;
}
