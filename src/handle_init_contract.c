#include "insurace_plugin.h"

static int find_selector(uint32_t selector, const uint32_t *selectors, size_t n, selector_t *out) {
    for (selector_t i = 0; i < n; i++) {
        if (selector == selectors[i]) {
            *out = i;
            return 0;
        }
    }
    return -1;
}

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));

    uint32_t selector = U4BE(msg->selector, 0);
    if (find_selector(selector, INSURACE_SELECTORS, NUM_SELECTORS, &context->selectorIndex)) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Set `next_param` to be the first field we expect to parse.
    // EDIT THIS: Adapt the `cases`, and set the `next_param` to be the first parameter you expect
    // to parse.
    switch (context->selectorIndex) {
        case INSURACE_C_COVER_M_BUY_COVER_V3:
            context->next_param = INSURACE_C_COVER_M_BUY_COVER_V3_P_PRODUCTS_OFFSET;
            break;
        case INSURACE_C_COVER_M_CANCEL_COVER:
            context->next_param = INSURACE_C_COVER_M_CANCEL_COVER_P_COVER_ID;
            break;
        case INSURACE_C_StakingV2Controller_M_stakeTokens:
            context->next_param = INSURACE_C_StakingV2Controller_M_stakeTokens_P_amount;
            break;
        case INSURACE_C_StakingV2Controller_M_proposeUnstakes:
            context->next_param = INSURACE_C_StakingV2Controller_M_proposeUnstakes_P_amount;
            break;
        case INSURACE_C_StakingV2Controller_M_withdrawTokens:
            context->next_param = INSURACE_C_StakingV2Controller_M_withdrawTokens_P_caller;
            break;
        case INSURACE_C_RewardController_M_unlockReward:
            context->next_param = INSURACE_C_RewardController_M_unlockReward_P_tokenList_OFFSET;
            break;
        case INSURACE_C_RewardController_M_withdrawReward:
            context->next_param = INSURACE_C_RewardController_M_withdrawReward_P_gameId;
            break;
        // Keep this
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
