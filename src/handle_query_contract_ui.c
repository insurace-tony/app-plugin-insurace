#include "insurace_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

// // Set UI for the "Send" screen.
// // EDIT THIS: Adapt / remove this function to your needs.
// static void set_send_ui(ethQueryContractUI_t *msg) {
//     strlcpy(msg->title, "Send", msg->titleLength);

//     const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
//     uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

//     // Converts the uint256 number located in `eth_amount` to its string representation and
//     // copies this to `msg->msg`.
//     amountToString(eth_amount, eth_amount_size, WEI_TO_ETHER, "ETH ", msg->msg, msg->msgLength);
// }

// // Set UI for "Receive" screen.
// // EDIT THIS: Adapt / remove this function to your needs.
// static void set_receive_ui(ethQueryContractUI_t *msg, const context_t *context) {
//     strlcpy(msg->title, "Receive Min.", msg->titleLength);

//     uint8_t decimals = context->decimals;
//     const char *ticker = context->ticker;

//     // If the token look up failed,
//     // use the default network ticker along with the default decimals.
//     if (!context->token_found) {
//         decimals = WEI_TO_ETHER;
//         ticker = msg->network_ticker;
//     }

//     amountToString(context->amount_received,
//                    sizeof(context->amount_received),
//                    decimals,
//                    ticker,
//                    msg->msg,
//                    msg->msgLength);
// }

// // Set UI for "Beneficiary" screen.
// // EDIT THIS: Adapt / remove this function to your needs.
// static void set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
//     strlcpy(msg->title, "Beneficiary", msg->titleLength);

//     // Prefix the address with `0x`.
//     msg->msg[0] = '0';
//     msg->msg[1] = 'x';

//     // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
//     // Setting it to `0` will make it work with every chainID :)
//     uint64_t chainid = 0;

//     // Get the string representation of the address stored in `context->beneficiary`. Put it in
//     // `msg->msg`.
//     getEthAddressStringFromBinary(
//         context->beneficiary,
//         msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
//         msg->pluginSharedRW->sha3,
//         chainid);
// }

// Set UI for First param screen
void set_first_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case INSURACE_C_COVER_M_BUY_COVER_V3:
            strlcpy(msg->title, "Buy", msg->titleLength);
            strlcpy(msg->msg, "Cover", msg->msgLength);
            break;
        case INSURACE_C_COVER_M_CANCEL_COVER:
            strlcpy(msg->title, "Cancel", msg->titleLength);
            strlcpy(msg->msg, "Cover", msg->msgLength);
            break;
        case INSURACE_C_StakingV2Controller_M_stakeTokens:
            strlcpy(msg->title, "Stake", msg->titleLength);
            strlcpy(msg->msg, "Tokens", msg->msgLength);
            break;
        case INSURACE_C_StakingV2Controller_M_proposeUnstakes:
            strlcpy(msg->title, "Propose", msg->titleLength);
            strlcpy(msg->msg, "Unstakes", msg->msgLength);
            break;
        case INSURACE_C_StakingV2Controller_M_withdrawTokens:
            strlcpy(msg->title, "Withdraw", msg->titleLength);
            strlcpy(msg->msg, "Tokens", msg->msgLength);
            break;
        case INSURACE_C_RewardController_M_unlockReward:
            strlcpy(msg->title, "Unlock", msg->titleLength);
            strlcpy(msg->msg, "Reward", msg->msgLength);
            break;
        case INSURACE_C_RewardController_M_withdrawReward:
            strlcpy(msg->title, "Withdraw", msg->titleLength);
            strlcpy(msg->msg, "Reward", msg->msgLength);
            break;
        default:
            PRINTF("Wrong selector index = %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            set_first_param_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
